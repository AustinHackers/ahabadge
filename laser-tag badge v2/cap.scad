$fn = 64;
h1 = 4.2;
r1 = 6;
w2 = 3.3;
h2 = 2.5;
h3 = 0.6;
difference() {
    cylinder(h=h1, r=r1);
    translate([0, 0, h1 - h3])
        cylinder(h=h3, r1=r1 / 2, r2=r1 * 0.9);
    translate([-w2 / 2, -w2 / 2, 0])
        cube([w2, w2, h2 + 0.4]);
}