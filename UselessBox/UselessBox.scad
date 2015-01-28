baseWidth = 53;
thick = 3;
servohole = 5.1/2;


module finger() //early version of the finger for a different design
{ difference(){
  union(){
			for ( i = [0 : 2] ) // two iterations, z = -1, z = 1
				{
    				hull(){
						rotate([90/5*i,0,0])
							translate([0, 50, i*(20-i*4)])
    						cube([18-i*2,18-i*4,18-i*2],center = true);
						rotate([90/5*(i+1),0,0])
							translate([0, 50, (i+1)*(20-(i+1)*4)])
    						cube([18-(i+1)*2,18-(i+1)*4,18-(i+1)*2], center = true);
						}
				}
			    translate([0,0,3])hull(){
					rotate([90/5*2+8,0,0])
						translate([0, 50-2, 2*(20-2*4)])
    					cube([18-3*3,18-3*4,18-3*2],center = true);
					rotate([90/5*(2+1),0,0])
						translate([0, 50, (2+1)*(20-(2+1)*4)])
    					cube([18-(3+1)*2,18-(3+1)*4,18-(3+1)*2], center = true);
					}
		}
	translate([0, 40, 0]) cube([18,40,28],center = true);
	translate([0, 50, 28/2]) cylinder(r=5/2,h=5,center = true);
	translate([0, 50, 28/2]) cylinder(r=2/2,h=40,center = true);
	translate([0, 50, 28/2+5/2+2+20/2]) cylinder(r=5/2,h=20,center = true);
  }
 
}



module FingerMid()
{
	 translate([0,0,12])cube([12,6,2],center = true);
	 translate([0,1,12])cube([6,9,2],center = true);
}

module FingerII()
{ 
	difference(){
     union(){ 
	  hull()
		{
 	   cube([12,12,2],center = true);
 	   FingerMid();
			}
  		  hull()
		{ 
  	  FingerMid();
		 translate([0,-3.5,18])cube([5,4,5],center = true);
		 translate([0,-1.5,26])cube([5,5,2],center = true);
		 translate([0,-1.5,23])cube([8,6,2],center = true);
		}
  	  hull() //nail
		{ 
		 translate([0,1,27])cube([5,1,2],center = true);	
		 translate([0,2,22])cube([8,1,2],center = true);
		}
		}
	 hull(){//deep slot
	 	translate([3,0,12])rotate([00,90,0]) cylinder(r=8/2,h=2,center = true); 
	 	translate([3,0,-10])rotate([00,90,0]) cylinder(r=8/2,h=2,center = true); 
		translate([3,-6,-4])rotate([00,90,0]) cylinder(r=8/2,h=2,center = true); 
		translate([3,-6,10])rotate([00,90,0]) cylinder(r=8/2,h=2,center = true); 		
		}
	 hull(){//deep slot
	 	translate([-3,0,12])rotate([00,90,0]) cylinder(r=8/2,h=2,center = true); 
	 	translate([-3,0,-10])rotate([00,90,0]) cylinder(r=8/2,h=2,center = true); 
		translate([-3,-6,-4])rotate([00,90,0]) cylinder(r=8/2,h=2,center = true); 
		translate([-3,-6,10])rotate([00,90,0]) cylinder(r=8/2,h=2,center = true); 		
		}
	 hull(){ //shallow Slot
	 	translate([0,0,2])rotate([00,90,0]) cylinder(r=8/2,h=2,center = true);
		translate([0,-6,2])rotate([00,90,0]) cylinder(r=8/2,h=2,center = true); 
	 	translate([0,0,-2])rotate([00,90,0]) cylinder(r=8/2,h=2,center = true); 
	
		}


 	translate([0,0,12])rotate([00,90,0]) cylinder(r=2/2,h=20,center = true);
   translate([0,0,2])rotate([00,90,0]) cylinder(r=2/2,h=20,center = true);
	//translate([5,0,11])rotate([00,90,0]) cylinder(r=4/2,h=4,center = true);
	//translate([-5,0,11])rotate([00,90,0]) cylinder(r=4/2,h=4,center = true);
	}
}

module ArcCam()
{arcSize = (7+9+12/2+10)/2;
	difference(){
		union(){
  			difference(){		
				cylinder(r=arcSize+3,h=1.5,center = true);
				cylinder(r=arcSize-3,h=1.5,center = true);
				translate([-arcSize-4,-arcSize-6,-1.5/2])cube([arcSize*2+8,arcSize+6,3]);
			}
		translate([arcSize,0,0])cylinder(r=3,h=1.5,center = true);
		translate([arcSize,0,3])cylinder(r=2.0,h=5,center = true,$fn =20);
		translate([-arcSize,0,0])cylinder(r=3,h=1.5,center = true);
		}
	translate([arcSize,0,-0.8])cylinder(r=1.4,h=12,center = true,$fn =20);
	translate([-arcSize,0,0])cylinder(r=1.2,h=1.5,center = true);
		
	}

}


module ArcCamLeg()
{arcSize = (7+9+12/2+10)/2;
	difference(){
		union(){
  			hull(){		
				translate([-10,0,0])cylinder(r=4,h=4,center = true);
				translate([10,0,0])cylinder(r=4,h=4,center = true);				
			}
		translate([-10,0,1])cylinder(r=3,h=6,center = true);
		
		}
	
		translate([10,0,0])cylinder(r=3.8,h=2,center = true);//slot
		translate([4,-4,-1])cube([6,8,2]);//slot
	
	translate([-10,0,1])cylinder(r=1.2,h=6,center = true);
	translate([10,0,-1])cylinder(r=1.2,h=2,center = true);
	translate([10,0,2.1])cylinder(r=1.2,h=2,center = true);
	translate([0,0,0])cylinder(r=2,h=4,center = true,$fn =20 ); //pivot
		
	}

}

module frame() //attempt to have a mini servo articulated finger
{  difference(){
		union(){
   		hull(){
		//	translate([-1,-6,0]) cube([2,2,35]);
			translate([-1,-6,35-11/2]) cube([2,12,11/2]);
			translate([0,0,35]) rotate([00,90,0]) cylinder(r=11/2,h=2,center = true);
			translate([-1,-6,18]) cube([2,2,8]); //angle shim after hull
			}
			translate([-10,-6,0]) cube([20,2,28]); //base plate
			translate([-10,-6,20]) cube([20,6,2]); //base plate end strength bar
			translate([-10,-6,0]) cube([2,6,25]); //servo mounts
			translate([8,-6,0]) cube([2,6,25]);   //servo mounts
	      translate([0,-6,5]) rotate([00,90,0]) cylinder(r=5/2,h=20,center = true); //pivot support
	      translate([0,-6,25]) rotate([00,90,0]) cylinder(r=5/2,h=20,center = true); // pivot support
		}
	translate([0,-6,5]) rotate([00,90,0]) cylinder(r=3.5/2,h=22,center = true); //pivot holes
	translate([0,-6,25]) rotate([00,90,0]) cylinder(r=3.5/2,h=22,center = true); // pivot holes
	translate([0,-6,5]) rotate([00,90,0]) cylinder(r=7/2,h=5,center = true); //nut holes
	translate([10,-6,25]) rotate([00,90,0]) cylinder(r=8/2,h=10,center = true); // nut holes
	translate([-10,-6,25]) rotate([00,90,0]) cylinder(r=8/2,h=10,center = true); // nut holes
	translate([0,0,35]) rotate([00,90,0]) cylinder(r=4/2,h=3,center = true); //finger pivot hole
	translate([(20-18)/2-10,-2,0]) cube([18,0.8,19]); //servo PCB slot 
	
	}
	
}

module frameII()
{  difference(){
		union(){
   		
		
			translate([-4,-20,0]) cube([32,40,2]); //base
			translate([-4,-20,0]) cube([6,40,35]); //side
			translate([27,-20,0]) cube([6,40,35]); //side
			translate([-4,-23/2,35-12.4]) cube([40,23,12.4]); //test

	

		}
		   translate([-2.5,-32.4/2,35-12.4]) cube([2.5,32.4,12.4]); //slot	
			translate([28.5,-32.4/2,35-12.4]) cube([1.3,32.4,12.4]); //slot	
			translate([-4,-23/2,35-12.4]) cube([40,23,12.4]); //box
			translate([-4,-34/2,2]) cube([40,34,19]);//void
	//			translate([-5,0,25+5]) rotate([00,90,0]) ServoShadow();
	//		translate([5,0,25+5]) rotate([00,270,0]) ServoShadow();
	}
	
}

module ServoShadow()
{  Servowidth = 11.4+.4;
	ServoHeight = 20.5;
	PostHeight = 3.2;  //gearhead height
  	translate([-Servowidth/2,-Servowidth/2,-PostHeight-ServoHeight]) cube([Servowidth,22.75,ServoHeight]);//body
	translate([-Servowidth/2,-Servowidth/2-(32.5-22.75)/2,-3.1-PostHeight-1]) cube([Servowidth,32.5,2]); //tab
	hull(){
		translate([0,0,0-3.2]) cylinder(r=Servowidth/2,h=3.2,center = false); //post/gearhead
		translate([0,Servowidth /2,0-3.2]) cylinder(r=6/2,h=3.2,center = false); //post/gearhead
	}
	translate([0,0,0]) cylinder(r=5.4/2,h=3.2);
   
}

module mount()
{  difference(){
		union(){

			translate([-35/2,-25,0]) cube([35,2,28]); //base plate
			translate([-35/2,-25,0]) cube([2,36,10]); //edge Mounts
			translate([35/2-2,-25,0]) cube([2,36,10]);   //edge Mounts
			translate([-35/2,-25,21]) cube([2,36,8]); //edge Mounts
			translate([35/2-2,-25,21]) cube([6,2,8]);   //edge Mounts
	 		translate([35/2+2,-23-2+1.5,32-28+2.5]) union(){ //servo stand
								difference(){
								translate([-2+4,-1.5,0]) cube([14,23+1.5+6,28]);
								translate([-2+4,-1.5+2,0+2]) cube([14,23+1-4.5,28-4]);
								}
								//translate([00,23-11,-5]) cube([5,11,5]);
								//translate([00,23-11,23]) cube([5,11,5]);
								
								}
			//translate([35/2,-6,25])rotate([270,0,90])ServoShadow();
		}
	translate([0,7,-2])rotate([-15])union(){//pivot point assembly
		translate([35/2,-6,25])rotate([270,0,90])ServoShadow();
		translate([0,-6,5]) rotate([00,90,0]) cylinder(r=4/2,h=35,center = true); //pivot holes
		translate([0,-6,25]) rotate([00,90,0]) cylinder(r=4/2,h=35,center = true); // pivot holes
		}

	//translate([0,-6,5]) rotate([00,90,0]) cylinder(r=7/2,h=22,center = true); //nut holes
	//translate([0,-6,25]) rotate([00,90,0]) cylinder(r=7/2,h=22,center = true); // nut holes
	translate([0,0,35]) rotate([00,90,0]) cylinder(r=2/2,h=3,center = true); //finger pivot hole
	translate([(20-18)/2-10,-2,0]) cube([18,0.8,19]); //servo PCB slot 
	
	}
	
}
module Pivots()
{   difference(){
		union(){
			hull(){
  				translate([0,0,1])  cylinder(r=9/2,h=2,center = true); //nut holes
				translate([0,20,1]) cylinder(r=9/2,h=2,center = true); // nut holes
				}
			translate([0,0,2.5])  cylinder(r=9/2,h=5,center = true); //nut holes
			translate([0,20,2.5]) cylinder(r=9/2,h=5,center = true); // nut holes
		}
	  		translate([0,0,2.5])  cylinder(r=4/2,h=5,center = true); //shaft holes
			translate([0,20,2.5]) cylinder(r=4/2,h=5,center = true); //shaft holes
		   translate([0,0,1.5])  cylinder(r=7/2,h=3,center = true); //bolt head holes
			translate([0,20,1.5+2]) cylinder(r=7/2,h=3,center = true); // bolt head holes		
	}
}

module PivotsII()
{   difference(){
		union(){
			hull(){
  				translate([0,0,1+4])  cylinder(r=8/2,h=2,center = true); //nut holes
				translate([0,20,1+4]) cylinder(r=8/2,h=2,center = true); // nut holes
				}
			translate([0,0,2.5])  cylinder(r=8/2,h=5,center = true); //nut holes
			translate([0,20,5]) cylinder(r=8/2,h=10,center = true); // nut holes
		}
	  		translate([0,0,2.5])  cylinder(r=4/2,h=5,center = true); //shaft holes
			translate([0,20,2.5+5]) cylinder(r=4/2,h=5,center = true); //shaft holes
		   translate([0,0,1.5+3])  cylinder(r=7/2,h=3,center = true); //bolt head holes
			translate([0,20,4]) cylinder(r=7/2,h=8,center = true); // bolt head holes		
	}
}

module LidLift()
{
  difference(){
	union(){
		translate([0,-6,0])cube([20,22,1.5]);
		hull(){
			translate([20/2,10/2,11.4/2]) rotate([90,90,0]) cylinder(r=5/2,h=4,center = true);
			translate([0,10/2-4/2,0])  cube([20,4,2]);
			}
		}
	translate([20/2,10/2,11.4/2]) rotate([90,90,0]) cylinder(r=4/2,h=4,center = true);
	for ( i = [0 : 3] )
		{translate([i*(20-1.5)/4+3,-3,.75]) cylinder(r=2/2,h=1.5,center = true);
		translate([i*(20-1.5)/4+3,12,.75]) cylinder(r=2/2,h=1.5,center = true);
		}
	}	
}

module switchPuck()
{
	difference(){
	union(){
		cylinder(r=18/2, h=0.8,center=true);
		translate([0,0,8/2])cylinder (r=9.5/2, h=8,center=true);
	}	
	//for ( r = [0 : 3] )rotate([0,0,90*r])translate([0,7,0,]) cylinder(r=1, h=1.8,center=true);
	translate([0,0,8/2])cylinder (r=5/2, h=12,center=true);
	translate([-8.5/2,-5.5/2,0.8,])cube([8.5,5.5,8]);
	translate([0,0,8/2+0.8])cylinder (r=6.5/2, h=8,center=true);
	}
}

module liftCam()
{
	difference(){
	union(){
		//cylinder(r=18/2, h=0.8,center=true);
		hull()
		 for ( r = [0 : 29] )rotate([0,0,r*6])translate([0,12/2+18*(1-cos(r*3)),0]) cylinder(r=2, h=4,center=true);
	}	
	//for ( r = [0 : 3] )rotate([0,0,90*r])translate([0,7,0,]) cylinder(r=1, h=1.8,center=true);
//	translate([0,0,8/2])cylinder (r=5/2, h=12,center=true);
//	translate([-8.5/2,-5.5/2,0.8,])cube([8.5,5.5,8]);
	translate([0,0,-1.5])cylinder (r=4/2, h=3,center=true);
	translate([0,0,1.5])cylinder (r=2/2, h=4,center=true);
	}
}
//mirror([0,0,1])liftCam();
//FingerII();
translate([25,-15,30])rotate([270,0,90])ArcCam();
translate([19,-15,30])rotate([270,0,90])ArcCam();
translate([22,-24,30])rotate([270,0,90]) difference(){
	ArcCam();
	translate([0,0,1])cylinder(h=5,r=20);
	}
translate([22,1,30])rotate([90,0,270])  ArcCamLeg();
frameII();
//translate([0,0,-33])frame();
translate([22,-42,30])rotate([90,0,180]) FingerII();


//translate([0,0,-33])mount();
//for ( i = [0 : 1] )translate([i*10,0,0]) PivotsII();

/*
difference(){
frameII();
translate([10,-25,-5])cube([50,50,100]);
}
translate([-4-12,-1.5+20,0])cube([30,1.5,30]);
*/

//LidLift();
//ServoShadow();
//switchPuck();