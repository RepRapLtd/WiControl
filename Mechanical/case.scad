xcen = 53;
ycen = 54;
dia = 3.5;

for(x=[-1,1])for(y=[-1,1])
translate([x*xcen/2,y*ycen/2,0])
cylinder(r=dia/2, h=20, center=true, $fn=20);

translate([-xcen/2,ycen/2,0])
{
	translate([9,-2,0])
		cylinder(r=1.7, h=20, center=true, $fn=20);
	translate([9+6.5,-2,0])
		cylinder(r=1.7, h=20, center=true, $fn=20);
	translate([9+6.5+6.3,-2,0])
		cylinder(r=1.7, h=20, center=true, $fn=20);
}