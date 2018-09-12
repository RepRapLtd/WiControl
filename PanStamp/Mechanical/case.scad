
//cubex=120; //71;
//cubey=130; //73;
cubex=71;
cubey=73;
xcen = cubex-18;
ycen = cubey-19;
dia = 3.5;
thick=8;
lid=true;
ribbonNotch=true;
//offx=cubex/2-71/2;
//offy=cubey/2-73/2;
offx = 0;
offy=0;

case();



module slot()
{
	translate([dia,0,0])
		cylinder(r=dia/2,h=60, center=true,$fn=20);
	translate([-dia,0,0])
		cylinder(r=dia/2,h=60, center=true,$fn=20);
	cube([2*dia,dia,60],center=true);	
}

module corner(box, rr)
{
	translate([offx, offy, 0])
	{
	for(x=[-1,1])for(y=[-1,1])
		translate([x*xcen/2,y*ycen/2,0])
		{
			if(box)
			{
				translate([2*x,2*y,-1])
					cube([12,12,thick], center=true);
			} else
			{
				cylinder(r=rr, h=40, center=true, $fn=20);
				if(lid)
				{
				translate([0,0,-thick/4-0.8])
				cylinder(r1=2.3*rr, r2=rr, h=2.5, center=true, $fn=20);
				}
			}
		}
}
}

module case()
{

difference()
{
	translate([offx, offy, 0])
		cube([cubex,cubey,thick], center=true);


	if(lid)
	{
		translate([-xcen/2,ycen/2,0])
		{
			translate([9,-2,0])
				cylinder(r=1.7, h=40, center=true, $fn=20);
			translate([9+6.5,-2,0])
			cylinder(r=1.7, h=40, center=true, $fn=20);
			translate([9+6.5+6.3,-2,0])
			cylinder(r=1.7, h=40, center=true, $fn=20);
		}
		translate([20-xcen/2,-ycen/2-3,0])
		{
			translate([-2,0,0])
					cylinder(r=1, h=40, center=true, $fn=20);
			translate([2,0,0])
			cylinder(r=1, h=40, center=true, $fn=20);
		}
		corner(box=false, rr=dia/2);
		if(ribbonNotch)
		{
			translate([-xcen/2-5,12,7.5])
				cube([10,17,10], center=true);
		}
	} else
	{
		translate([xcen/2,-ycen/2,12.5-1-4])
		{
			translate([-30,-2,0])
				rotate([90,0,0])
					cylinder(r=2.5, h=40, center=true, $fn=20);

			translate([-30+10+10,-2,0])
				rotate([90,0,0])
					cylinder(r=2.5, h=40, center=true, $fn=20);
		}

		translate([-xcen/2,ycen/2,12.5-14])
					translate([30-10,-2,0])
				rotate([90,0,0])
					cylinder(r=1, h=40, center=true, $fn=20);

		translate([xcen/4,-ycen/4,0])
			slot();
		translate([-xcen/4,ycen/4,0])
			rotate([0,0,90])
				slot();
		corner(box=false, rr=0.8*dia/2);
	}


translate([offx, offy, 0])
	difference()
	{
		translate([0,0,3])
		cube([cubex-6,cubey-6,thick], center=true, r=dia/2);
		corner(box=true, rr=dia/2);
	}

}

}






