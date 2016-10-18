extern crate kiss3d;
extern crate nalgebra as na;
extern crate time;

use kiss3d::window::Window;
use kiss3d::light::Light;

mod leg;
use leg::Leg;

use std::fs::File;
use std::io::prelude::{Read};
use std::path::Path;

use std::vec::Vec;

use std::f32::consts::PI;

macro_rules! try_with_yolo{
    ($expr:expr) =>
    {
        match $expr
        {
            _ => {}
        }
    }
}

struct Robot
{
    legs: Vec<Leg>
}

impl Robot
{
    pub fn new(window: &mut Window) -> Robot 
    {
        let mut legs = vec!();

        for i in 0..5
        {
            legs.push(Leg::new(window));
        }

        Robot {
            legs: legs
        }
    }
}

//Panics if file is wrong, maybe change?
fn read_target_angles() -> Vec<Vec<f32>>
{
    let file_dir = String::from("/tmp/hexsim");
    let filepath = file_dir.clone() + "/leg_input";

    try_with_yolo!(std::fs::create_dir(&file_dir));

    //Opening the file
    let mut file = File::open(filepath).unwrap();

    //Reading the file into a string
    let mut s = String::new();
    file.read_to_string(&mut s).unwrap();
    println!("{:?}", file);
    println!("{:?}", s);

    //Parsing the data
    let mut result = vec!();

    let lines = s.split('\n');

    for line in lines
    {
        if line == ""
        {
            continue
        }
        let angles = line.split(',');

        result.push(angles.map(|elem|{
                elem.parse::<f32>().unwrap() / 180.0 * PI
            }).collect())
    }


    return result
}

fn main() {
    let mut window = Window::new("Kiss3d: cube");

    window.set_light(Light::StickToCamera);

    let mut test_leg = Leg::new(&mut window);
    test_leg.set_target_angles(vec!(0.,0.2,-0.2));

    let mut old_time = time::precise_time_s() as f32;
    while window.render() 
    {
        let new_time = time::precise_time_s() as f32;
        let delta_time = new_time - old_time;
        old_time = new_time;

        let target_angles = read_target_angles();

        for angle in &target_angles
        {
            println!("{:?}", angle);
        }

        test_leg.set_target_angles(target_angles[0].clone());

        test_leg.update(delta_time);
    }
}
