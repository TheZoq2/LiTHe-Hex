#[macro_use]
extern crate scad_generator;
extern crate scad_util as su;

use scad_generator::*;

fn ir_screwholes(depth: f32) -> ScadObject
{
    let screw_distance = 38.;
    let screw_diameter = 4.;

    //TODO add nut
    let screwhole = scad!(Cylinder(depth, Diameter(screw_diameter)));

    let translated = scad!(Translate(vec3(screw_distance, 0., 0.));screwhole);

    scad!(Union;
    {
        translated.clone(),
        scad!(Mirror(vec3(1., 0., 0.)); translated)
    })
}

pub fn main()
{
    let mut sfile = ScadFile::new();

    sfile.set_detail(50);

    sfile.add_object(ir_screwholes(10.));

    //Save the scad code to a file
    sfile.write_to_file(String::from("out.scad"));
}
