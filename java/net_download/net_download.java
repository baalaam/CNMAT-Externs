// Thanks to Ali Momeni for some of this code.

import com.cycling74.max.*;
import com.cycling74.jitter.*;

import java.io.*;
import java.util.*;
import java.net.*;
import javax.swing.text.*;
import javax.swing.text.html.*;
import java.awt.image.BufferedImage;
import javax.imageio.ImageIO;

public class net_download extends MaxObject
{	
	public void version(){
		post("net_download Version 1.0, by John MacCallum.\nCopyright (c) 2007 Regents of the University of California.  All rights reserved.");
	}
			
	//List q = Collections.synchronizedList(new LinkedList());
	LinkedList q = new LinkedList();
	int numActiveConnections = 0;
	int maxNumConnections = 10;
	MaxClock c;
	String tmpdir = null;

	public net_download(Atom[] args){
		declareOutlets(new int[]{DataTypes.ALL, DataTypes.ALL});	
		c = new MaxClock(new Callback(this, "dolist")); 
		c.delay(100);
		
		// this block of code is to make sure that the tmp file we use is not an alias.
		// on mac os x System.getProperty("java.io.tmpdir") returns /tmp which is an
		// alias that points to /private/tmp
		File f = null;
		try{f = File.createTempFile("tmp","");}
		catch(Exception e){}
		try{tmpdir = f.getCanonicalPath();}
		catch(Exception e){
			error("net_download: couldn't get the default system tmp path");
		}
		tmpdir = tmpdir.substring(0, tmpdir.lastIndexOf(System.getProperty("file.separator")));	
		post("tmpdir: " + tmpdir);
		
		version();
	}	
	
	public void get_html(Atom[] args){
		post("� get_html " + args[0].toString());
		ArrayList ar = new ArrayList();
		ar.add(new Integer(1));
		ar.add(args);
		synchronized(this.q){
			q.add(ar);
		}
	}
	
	public void get_image(Atom[] args){
		post("� get_image " + args[0].toString());
		ArrayList ar = new ArrayList();
		ar.add(new Integer(2));
		ar.add(args);
		synchronized(this.q){
			q.add(ar);
		}
	}
	
	public void get_file(Atom[] args){
		post("� get_file " + args[0].toString());
		ArrayList ar = new ArrayList();
		ar.add(new Integer(3));
		ar.add(args);
		synchronized(this.q){
			q.add(ar);
		}
	}
	
	public void bang(){
		dolist();
	}
	
	private void dolist(){
		//post("numActiveConnections: " + numActiveConnections);
		//post("q.size = " + q.size());
		
		outlet(1, new Atom[]{Atom.newAtom("/numActiveConnections"), Atom.newAtom(numActiveConnections)});
		outlet(1, new Atom[]{Atom.newAtom("/remainingFiles"), Atom.newAtom(q.size())});
		if(q.size() == 0 && numActiveConnections == 0)
			outlet(1, "/done");
		if(q.size() == 0 || numActiveConnections >= maxNumConnections) {
			c.delay(100);
			return;
		}
	
		Atom[] at;
		ArrayList ar;
		Integer i;
		synchronized(this.q){
			ar = (ArrayList)(q.poll());
			i = (Integer)ar.get(0);
			at = (Atom[])ar.get(1);
		}
		switch(i){
			case 1:
				doget_html(at);
				break;
			case 2:
				doget_image(at);
				break;
			case 3:
				doget_file(at);
				break;
		}
		c.delay(100);
	}
	
	private void doget_html(Atom[] args){
		String url;
		String tmp;
		ArrayList tags = new ArrayList();
		ArrayList attributes = new ArrayList();
		int counter = 2;
		
		url = args[0].getString();
		
		if(args.length == 1){
			HTML.Tag[] t = HTML.getAllTags();
			for(int i = 0; i < t.length; i++){
				tags.add(t[i].toString());
			}
		}
		if(args.length == 2){
			HTML.Attribute[] t = HTML.getAllAttributeKeys();
			for(int i = 0; i < t.length; i++){
				attributes.add(t[i].toString());
			}
		}
		
		if(args[1].getString().compareTo("tags") == 1) return;
		
		tmp = args[2].getString();
		while(tmp.compareTo("attributes") != 0){
			tags.add(tmp);
			tmp = args[++counter].getString();
		}
		for(int i = ++counter; i < args.length; i++){
			attributes.add(args[i].getString());
		}
		doget_html(url, tags, attributes);
	}
	
	private void doget_html(final String url, final ArrayList tags, final ArrayList attributes){
		Thread t = new Thread() 
			{
			public void run()
				{
				++numActiveConnections;
				Reader reader;
				URLConnection conn;
				try{
					conn = new URL(url).openConnection();
					conn.addRequestProperty("User-Agent", "Mozilla/4.76"); 
				   
					reader = new InputStreamReader(conn.getInputStream());
				} catch (Exception e){
					--numActiveConnections;
					outletBang(1);
					e.printStackTrace();
					return;
				}
				
				EditorKit kit = new HTMLEditorKit();
				Document doc = kit.createDefaultDocument();

				// post("searching..."+str);
				// The Document class does not yet handle charset's properly.
				doc.putProperty("IgnoreCharsetDirective", Boolean.TRUE);
				try {
					// Parse the HTML.
					kit.read(reader, doc, 0);

					// Iterate through the elements of the HTML document.
					ElementIterator it = new ElementIterator(doc);
					javax.swing.text.Element elem;
					
					while ((elem = it.next()) != null) {
						for(int i = 0; i < tags.size(); i++){
							SimpleAttributeSet s = (SimpleAttributeSet)elem.getAttributes().getAttribute(HTML.getTag((String)tags.get(i)));
								
							if (s != null) {				
								for(int j = 0; j < attributes.size(); j++){
									String link = s.getAttribute(HTML.getAttributeKey((String)attributes.get(j))).toString();
									//outlet(1, url);
									if(((String)attributes.get(j)).compareTo("href") == 0){
										if(link.contains("http://")){
											outlet(0, new Atom[]{Atom.newAtom((String)tags.get(i)), Atom.newAtom((String)attributes.get(j)), 
												Atom.newAtom(link.toString())});
										} else {
											outlet(0, new Atom[]{Atom.newAtom((String)tags.get(i)), Atom.newAtom((String)attributes.get(j)), 
												Atom.newAtom(url), Atom.newAtom(link.toString())});
										}
									} else {
										outlet(0, new Atom[]{Atom.newAtom((String)tags.get(i)), Atom.newAtom((String)attributes.get(j)), 
												Atom.newAtom(link.toString())});
									}
								}
							}
						}				
					}
				outletBang(1);
				
				} catch (Exception e) {
					e.printStackTrace();
				}
				--numActiveConnections;
				//dolist();
			}
		};
		t.start();
	}
	
	private void doget_image(final Atom[] args){
		Thread t = new Thread() 
			{
			public void run()
				{
					++numActiveConnections;
					String imageURL = args[0].toString();
					String outputPath = null;
					JitterMatrix jitmat;
					URL url;
					
					if(args.length > 1)
						outputPath = args[1].toString();
						
					BufferedImage image = null;
					try{ 
						url = new URL(imageURL);
						image = ImageIO.read(url);
						jitmat = new JitterMatrix(image);
					}catch (IOException e) {
						get_image(args); // if we can't connect, add this url back on to the list
						post("� error: net_download: couldn't download " + imageURL);
						--numActiveConnections;
						outletBang(1);
						e.printStackTrace();
						return;
					}
					outlet(0,"image",new Atom[]{Atom.newAtom("jit_matrix"),Atom.newAtom(jitmat.getName())});
					
					//if(outputPath != null){
						String tmp = url.toString();
						String[] tmpar = tmp.split("/");
						File fp;
						try{
							fp = new File(tmpdir + File.separator + tmpar[tmpar.length - 1]);//File.createTempFile(imageURL.substring(imageURL.lastIndexOf("/") + 1), "");
						} catch(Exception e){
							error("net_download: couldn't create output file for " + imageURL);
							e.printStackTrace();
							return;
						}
						try{
							ImageIO.write(image, "jpg", fp);
							outlet(0, new Atom[]{Atom.newAtom("image_filepath"), Atom.newAtom(tmpdir), 
								Atom.newAtom(tmpar[tmpar.length - 1])});
						} catch(Exception e){
							e.printStackTrace();
						}
					//}
					outletBang(1);
					--numActiveConnections;
					//dolist();
				}
			};
		t.start();
	}
	
	private void doget_file(final Atom[] args){
		final String url = args[0].toString();
		Thread t = new Thread() 
		{
		public void run()
			{
			++numActiveConnections;
			BufferedInputStream iostream = null;
			URLConnection conn;
			ArrayList<Byte> buffer = new ArrayList();
			int b = 0;
			//File out = new File("/Users/johnmac/porkbutt.cdf");
			File fp = null;
			FileOutputStream iostreamout = null;
			int offset = 0;
			try{
				fp = File.createTempFile(url.substring(url.lastIndexOf("/") + 1), "");
				iostreamout = new FileOutputStream(fp);
			} catch(Exception e){
				e.printStackTrace();
			}
			
			try{
				conn = new URL(url).openConnection();
				conn.addRequestProperty("User-Agent", "Mozilla/4.76"); 
				iostream = new BufferedInputStream(conn.getInputStream());
			}catch(Exception e){e.printStackTrace();}
			
			try{b = iostream.read();}
			catch(Exception e){e.printStackTrace();}
			int counter = 0;
			while(b >= 0){
				try{
					buffer.add(new Byte((byte)b));
					b = iostream.read();
					++offset;
				} catch (Exception e){
					e.printStackTrace();
					return;
				}
			}
			
			byte[] buffer_byte = new byte[buffer.size()];
			for(int i = 0; i < buffer.size(); i++)
				buffer_byte[i] = ((Byte)buffer.get(i)).byteValue();
			try{
				iostreamout.write(buffer_byte);
			} catch (Exception e){
				e.printStackTrace();
				return;
			}
			try{iostreamout.close();}
			catch(Exception e){e.printStackTrace();}
			outlet(0, new Atom[]{Atom.newAtom("filepath"), Atom.newAtom(fp.getAbsolutePath())});
			post("finished downloading\n\t" + url);
			outletBang(1);
			--numActiveConnections;
			//dolist();
			}
		};
		t.start();
	}
		
	public void clear(){
	}
		
	public void notifyDeleted() {
		c.release();
	}
	
}