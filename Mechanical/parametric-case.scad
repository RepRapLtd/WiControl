

Case(box=[192,90,30], wall=2.5, screws=3.5/2, countersink=false);


module Holes(box, wall)
{
   union()
	{

// Each pattern is looking at each face; RH XY coordinates; bottom left corner = (0,0)
// except the base, which is looking from on-top/inside.

// Base

	translate([0,0,-box.z/2]) 
	translate([-box.x/2,-box.y/2,0])
	{
		translate([15+4, 15.5+4, 0])
			cylinder(r=1.3, h=4*wall, center=true, $fn=20);
		translate([15+4, 15.5+4+49, 0])
			cylinder(r=1.3, h=4*wall, center=true, $fn=20);
		translate([15+4+58, 15.5+4, 0])
			cylinder(r=1.3, h=4*wall, center=true, $fn=20);
		translate([15+4+58, 15.5+4+49, 0])
			cylinder(r=1.3, h=4*wall, center=true, $fn=20);

		translate([35, 33, 0])
			cube([4,12,4*wall], center=true);
		translate([box.x-35, box.y-33, 0])
			cube([12,4,4*wall], center=true);	
	}

// Low X face

	translate([-box.x/2,0,0])rotate([0,-90,0])rotate([0,0,-90])translate([-box.y/2, -box.z/2, 0])
	{
		//translate([15, 15, 0])
		//	cylinder(r=2, h=4*wall, center=true);

	}

// High X face

	translate([box.x/2,0,0])rotate([0,90,0])rotate([0,0,90])translate([-box.y/2, -box.z/2, 0])
	{
		//translate([15, 19, 0])
		//	cube([4,15,4*wall], center=true);

	}

// Low Y face

	translate([0,-box.y/2,0])rotate([90,0,0])translate([-box.x/2, -box.z/2, 0])
	{
		translate([15+4+7, 12, 0])
			cube([14,10,4*wall], center=true);
		translate([15+4+102, 10, 0])
			cube([13,14,4*wall], center=true);
	}



// High Y face

	translate([0,box.y/2,0])rotate([-90,0,0])rotate([0,0,180])translate([-box.x/2, -box.z/2, 0])
	{
		translate([box.x-(15+4+152), 24+wall, 0])
			cylinder(r=0.7, h=4*wall, center=true, $fn=20);		

	}

	}

}

module Corners(box, rad, solid, countersink)
{
	xcen = box.x-18;
	ycen = box.y-19;

	for(x=[-1,1])for(y=[-1,1])
		translate([x*xcen/2,y*ycen/2,0])
		{
			if(solid)
			{
				translate([2*x,2*y,0])
					cube([12,12,box.z-2], center=true);
			} else
			{
				cylinder(r=rad, h=box.z*3, center=true, $fn=20);
				if(countersink)
				{
					translate([0,0,-box.z/2+0.8])
						cylinder(r1=2.3*rad, r2=rad, h=2.5, center=true, $fn=20);
				}
			}
		}
}

module Case(box=[60,40,30], wall=2.5, screws=1.75, countersink=false)
{
	difference()
	{
		union()
		{
			difference()
			{
				cube(box, center=true);
				translate([0,0,wall])
					cube([box.x-2*wall, box.y-2*wall, box.z], center=true);
			}
			Corners(box, screws, true, countersink);
		}
		Holes(box, wall);
		Corners(box, screws, false, countersink);
	}

}