{
	"patcher" : 	{
		"fileversion" : 1,
		"rect" : [ 486.0, 166.0, 1208.0, 760.0 ],
		"bglocked" : 0,
		"defrect" : [ 486.0, 166.0, 1208.0, 760.0 ],
		"openrect" : [ 0.0, 0.0, 0.0, 0.0 ],
		"openinpresentation" : 0,
		"default_fontsize" : 20.0,
		"default_fontface" : 0,
		"default_fontname" : "Arial",
		"gridonopen" : 0,
		"gridsize" : [ 15.0, 15.0 ],
		"gridsnaponopen" : 0,
		"toolbarvisible" : 1,
		"boxanimatetime" : 200,
		"imprint" : 0,
		"enablehscroll" : 1,
		"enablevscroll" : 1,
		"devicewidth" : 0.0,
		"boxes" : [ 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "tracing",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 410.0, 1038.0, 150.0, 29.0 ],
					"id" : "obj-31",
					"fontname" : "Arial",
					"fontsize" : 20.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "button",
					"presentation_rect" : [ 370.0, 928.0, 0.0, 0.0 ],
					"numinlets" : 1,
					"numoutlets" : 1,
					"patching_rect" : [ 354.0, 1030.0, 20.0, 20.0 ],
					"outlettype" : [ "bang" ],
					"id" : "obj-25"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.build /a 1 5 7 4 /verbose 1",
					"numinlets" : 2,
					"numoutlets" : 1,
					"patching_rect" : [ 353.0, 1071.0, 257.0, 29.0 ],
					"outlettype" : [ "FullPacket" ],
					"id" : "obj-27",
					"fontname" : "Arial",
					"fontsize" : 20.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "button",
					"presentation_rect" : [ 166.0, 925.0, 0.0, 0.0 ],
					"numinlets" : 1,
					"numoutlets" : 1,
					"patching_rect" : [ 175.0, 1026.0, 20.0, 20.0 ],
					"outlettype" : [ "bang" ],
					"id" : "obj-23"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.build /a 1 5 7 4",
					"numinlets" : 1,
					"numoutlets" : 1,
					"patching_rect" : [ 174.0, 1067.0, 158.0, 29.0 ],
					"outlettype" : [ "FullPacket" ],
					"id" : "obj-21",
					"fontname" : "Arial",
					"fontsize" : 20.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/n 6.5 \n/n/factorial 1055.742188 \n",
					"linecount" : 2,
					"presentation_rect" : [ 68.973145, 740.0, 0.0, 0.0 ],
					"numinlets" : 2,
					"numoutlets" : 1,
					"patching_rect" : [ 40.973145, 796.0, 290.0, 48.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-14",
					"fontname" : "Arial",
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"fontsize" : 18.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.route /n/n",
					"numinlets" : 1,
					"numoutlets" : 2,
					"patching_rect" : [ 39.0, 750.0, 112.0, 29.0 ],
					"outlettype" : [ "", "FullPacket" ],
					"id" : "obj-11",
					"fontname" : "Arial",
					"fontsize" : 20.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.expr /n/factorial = 1 /n/n= /n",
					"numinlets" : 1,
					"numoutlets" : 1,
					"patching_rect" : [ 42.0, 615.0, 245.0, 27.0 ],
					"outlettype" : [ "FullPacket" ],
					"id" : "obj-4",
					"fontname" : "Arial",
					"fontsize" : 18.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.expr /n/factorial *= /n/n--",
					"numinlets" : 1,
					"numoutlets" : 1,
					"patching_rect" : [ 138.973145, 702.0, 218.0, 27.0 ],
					"outlettype" : [ "FullPacket" ],
					"id" : "obj-7",
					"fontname" : "Arial",
					"fontsize" : 18.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.if !(/n/n > 0)",
					"numinlets" : 1,
					"numoutlets" : 2,
					"patching_rect" : [ 40.973137, 664.0, 117.0, 27.0 ],
					"outlettype" : [ "FullPacket", "FullPacket" ],
					"id" : "obj-8",
					"fontname" : "Arial",
					"fontsize" : 18.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "button",
					"numinlets" : 1,
					"numoutlets" : 1,
					"patching_rect" : [ 82.0, 1020.0, 20.0, 20.0 ],
					"outlettype" : [ "bang" ],
					"id" : "obj-50"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.expr /a=4",
					"numinlets" : 1,
					"numoutlets" : 1,
					"patching_rect" : [ 43.0, 1076.0, 111.0, 29.0 ],
					"outlettype" : [ "FullPacket" ],
					"id" : "obj-48",
					"fontname" : "Arial",
					"fontsize" : 20.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "You can signal the OSC address to compute from using an abstraction to wrap the core recursion like this:",
					"linecount" : 3,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 16.0, 926.0, 380.0, 75.0 ],
					"id" : "obj-47",
					"fontname" : "Arial",
					"fontsize" : 20.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "We can create a temporary variable so\nas to leave /n untouched:\n",
					"linecount" : 2,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 7.0, 551.0, 555.0, 52.0 ],
					"id" : "obj-45",
					"fontname" : "Arial",
					"fontsize" : 20.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/a 1 5 7 4 \n/a/factorial 1 120 5040 24 \n",
					"linecount" : 2,
					"numinlets" : 2,
					"numoutlets" : 1,
					"patching_rect" : [ 100.973145, 1161.0, 553.0, 48.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-43",
					"fontname" : "Arial",
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"fontsize" : 18.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.factorial /a",
					"numinlets" : 1,
					"numoutlets" : 1,
					"patching_rect" : [ 43.0, 1114.0, 119.0, 29.0 ],
					"outlettype" : [ "FullPacket" ],
					"id" : "obj-42",
					"fontname" : "Arial",
					"fontsize" : 20.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "20.",
					"numinlets" : 2,
					"numoutlets" : 1,
					"patching_rect" : [ 384.0, 169.0, 36.0, 25.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-41",
					"fontname" : "Arial",
					"fontsize" : 18.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "20",
					"numinlets" : 2,
					"numoutlets" : 1,
					"patching_rect" : [ 185.0, 129.0, 31.0, 25.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-40",
					"fontname" : "Arial",
					"fontsize" : 18.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "15",
					"numinlets" : 2,
					"numoutlets" : 1,
					"patching_rect" : [ 386.0, 137.0, 31.0, 25.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-39",
					"fontname" : "Arial",
					"fontsize" : 18.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "10",
					"numinlets" : 2,
					"numoutlets" : 1,
					"patching_rect" : [ 347.0, 122.0, 31.0, 25.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-38",
					"fontname" : "Arial",
					"fontsize" : 18.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "edge case values",
					"linecount" : 2,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 246.0, 164.0, 138.0, 48.0 ],
					"id" : "obj-37",
					"fontname" : "Arial",
					"fontsize" : 18.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "regular case",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 5.0, 61.0, 137.0, 27.0 ],
					"id" : "obj-35",
					"fontname" : "Arial",
					"fontsize" : 18.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "edge case types",
					"linecount" : 2,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 197.0, 14.0, 138.0, 48.0 ],
					"id" : "obj-34",
					"fontname" : "Arial",
					"fontsize" : 18.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "4 7 2",
					"numinlets" : 2,
					"numoutlets" : 1,
					"patching_rect" : [ 270.0, 58.0, 51.0, 25.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-32",
					"fontname" : "Arial",
					"fontsize" : 18.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "6.",
					"numinlets" : 2,
					"numoutlets" : 1,
					"patching_rect" : [ 157.0, 28.0, 32.0, 25.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-29",
					"fontname" : "Arial",
					"fontsize" : 18.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "fred",
					"numinlets" : 2,
					"numoutlets" : 1,
					"patching_rect" : [ 162.0, 63.0, 42.0, 25.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-28",
					"fontname" : "Arial",
					"fontsize" : 18.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "6.5",
					"numinlets" : 2,
					"numoutlets" : 1,
					"patching_rect" : [ 221.0, 57.0, 36.0, 25.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-26",
					"fontname" : "Arial",
					"fontsize" : 18.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "-2",
					"numinlets" : 2,
					"numoutlets" : 1,
					"patching_rect" : [ 310.0, 124.0, 32.0, 25.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-24",
					"fontname" : "Arial",
					"fontsize" : 18.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "0",
					"numinlets" : 2,
					"numoutlets" : 1,
					"patching_rect" : [ 270.0, 123.0, 31.0, 25.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-22",
					"fontname" : "Arial",
					"fontsize" : 18.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/n -0.5 \n/n/factorial 1055.742188 \n",
					"linecount" : 3,
					"numinlets" : 2,
					"numoutlets" : 1,
					"patching_rect" : [ 40.973145, 408.0, 157.0, 69.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-19",
					"fontname" : "Arial",
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"fontsize" : 18.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "1",
					"numinlets" : 2,
					"numoutlets" : 1,
					"patching_rect" : [ 230.0, 123.0, 31.0, 25.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-17",
					"fontname" : "Arial",
					"fontsize" : 18.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.expr /n/factorial = 1",
					"numinlets" : 1,
					"numoutlets" : 1,
					"patching_rect" : [ 88.0, 250.0, 179.0, 27.0 ],
					"outlettype" : [ "FullPacket" ],
					"id" : "obj-16",
					"fontname" : "Arial",
					"fontsize" : 18.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "OSC messages without /n or outside numerical bounds are passed through.\n\n",
					"linecount" : 5,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 398.0, 230.0, 188.0, 110.0 ],
					"id" : "obj-15",
					"fontname" : "Arial",
					"fontsize" : 18.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "This tutorial illustrates some classical defensive programming practices with o. using the factorial function as an example.\n\nAdrian Freed 2011 ",
					"linecount" : 5,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 467.0, 11.0, 405.0, 110.0 ],
					"id" : "obj-12",
					"fontname" : "Arial",
					"fontsize" : 18.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "4",
					"numinlets" : 2,
					"numoutlets" : 1,
					"patching_rect" : [ 25.0, 113.0, 31.0, 25.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-10",
					"fontname" : "Arial",
					"fontsize" : 18.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "5",
					"numinlets" : 2,
					"numoutlets" : 1,
					"patching_rect" : [ 61.0, 100.0, 31.0, 25.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-6",
					"fontname" : "Arial",
					"fontsize" : 18.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.expr /n/factorial *= /n--",
					"numinlets" : 1,
					"numoutlets" : 1,
					"patching_rect" : [ 163.973145, 335.0, 204.0, 27.0 ],
					"outlettype" : [ "FullPacket" ],
					"id" : "obj-3",
					"fontname" : "Arial",
					"fontsize" : 18.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.if !(/n > 0)",
					"numinlets" : 1,
					"numoutlets" : 2,
					"patching_rect" : [ 86.973137, 299.0, 102.0, 27.0 ],
					"outlettype" : [ "FullPacket", "FullPacket" ],
					"id" : "obj-2",
					"fontname" : "Arial",
					"fontsize" : 18.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.build /n 1",
					"numinlets" : 1,
					"numoutlets" : 1,
					"patching_rect" : [ 84.973137, 180.0, 100.0, 27.0 ],
					"outlettype" : [ "FullPacket" ],
					"id" : "obj-1",
					"fontname" : "Arial",
					"fontsize" : 18.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "                 The core recursion\n\n\n\n\n\n\n\n",
					"linecount" : 8,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 48.0, 217.0, 349.0, 172.0 ],
					"id" : "obj-13",
					"fontname" : "Arial",
					"bgcolor" : [ 0.011765, 1.0, 0.803922, 1.0 ],
					"fontsize" : 18.0
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-48", 0 ],
					"destination" : [ "obj-42", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-42", 0 ],
					"destination" : [ "obj-43", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-21", 0 ],
					"destination" : [ "obj-42", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-27", 0 ],
					"destination" : [ "obj-42", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-25", 0 ],
					"destination" : [ "obj-27", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-23", 0 ],
					"destination" : [ "obj-21", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-1", 0 ],
					"destination" : [ "obj-4", 0 ],
					"hidden" : 0,
					"midpoints" : [ 94.473137, 211.0, 20.0, 211.0, 29.0, 557.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-50", 0 ],
					"destination" : [ "obj-48", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-11", 1 ],
					"destination" : [ "obj-14", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-8", 0 ],
					"destination" : [ "obj-11", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-4", 0 ],
					"destination" : [ "obj-8", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-8", 1 ],
					"destination" : [ "obj-7", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-7", 0 ],
					"destination" : [ "obj-8", 0 ],
					"hidden" : 0,
					"midpoints" : [ 148.473145, 745.0, 418.986572, 745.0, 418.986572, 655.0, 50.473137, 655.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-3", 0 ],
					"destination" : [ "obj-2", 0 ],
					"hidden" : 0,
					"midpoints" : [ 173.473145, 380.0, 378.986572, 380.0, 378.986572, 290.0, 96.473137, 290.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-2", 1 ],
					"destination" : [ "obj-3", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-16", 0 ],
					"destination" : [ "obj-2", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-2", 0 ],
					"destination" : [ "obj-19", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-6", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-10", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-1", 0 ],
					"destination" : [ "obj-16", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-17", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-22", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-24", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-26", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-28", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-29", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-32", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-38", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-39", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-40", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-41", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
 ]
	}

}