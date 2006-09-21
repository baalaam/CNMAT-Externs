inlets=1;
outlets=1;
//Shepard tone synthesis 1.0 
//Adrian Freed 2005,2006 Copyright UC REgents. All Rights Reserved.
// 


var n=8; // number of partials
var f0=80; // frequency f0 in Hz
var f=1; // normalized frequency
var offset=0;
function bang()
{
	msg_float(1.0594633);
}
function msg_float(ff)
{
	f *=ff;
	var pi = Math.pow(1.0594633,12);
//	post(pi);
	var l=new Array();

	while(f<1)
	{
		f = f-1+pi;
		offset = (offset+n-1)%n;
	}
	while(f>pi )
	{
		f = f-pi+1;
		offset = (offset+1)%n;
	
	}


	var freq;
	for(var j=0;j<n;++j)
	{
		freq = f* Math.pow(pi,j) ;
		l[((j+n-offset)%n)*2] = f0*freq;
	//l[((j+n-offset)%n)*2+1] = Math.exp(-4/n*(f+j-n/2)*(f+j -n/2)); // gaussian works, many hat functions do in fact
		l[((j+n-offset)%n)*2+1] = Math.pow(10,0.075*(80*0.5*(1+Math.cos(3.14159265358979*(f+j-n/2)/((n+pi)/2)))-80)); // Shepard used a raised cosine
	}
	outlet(0,l);
}


function pitch(f)
{
	if(f>0 && f<1000000)
		f0 = f;
}
function partials(p)
{
	if(p>0 && p<10000000)
		n = p;
}