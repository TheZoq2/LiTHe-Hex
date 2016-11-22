extern crate kiss3d;
extern crate nalgebra as na;
extern crate time;

use kiss3d::window::Window;
use kiss3d::light::Light;

mod leg;
mod constants;
use leg::Leg;

use std::fs::{File};
use std::io::prelude::{Read};
use std::io::Write;

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

        let leg_positions = vec!(
                            na::Vector3::new(-6., 0., 12.) * constants::UNIT_SCALE,
                            na::Vector3::new(6., 0., 12.) * constants::UNIT_SCALE,
                            na::Vector3::new(-10., 0., 0.) * constants::UNIT_SCALE,
                            na::Vector3::new(10., 0., 0.) * constants::UNIT_SCALE,
                            na::Vector3::new(-6., 0., -12.) * constants::UNIT_SCALE,
                            na::Vector3::new(6., 0., -12.) * constants::UNIT_SCALE,
                        );

        let leg_angles = vec!(-PI/4., PI/4., -PI/2., PI/2., -3.*PI/4., 3.*PI/4.);

        for i in 0..6
        {
            legs.push(Leg::new(window, leg_angles[i], leg_positions[i]));
        }

        Robot {
            legs: legs
        }
    }

    pub fn update(&mut self, delta_time: f32) 
    {
        for leg in &mut self.legs
        {
            leg.update(delta_time);
        }
    }

    pub fn set_target_angles(&mut self, angles: Vec<Vec<f32>>)
    {
        assert_eq!(angles.len(), self.legs.len());

        for i in 0..self.legs.len()
        {
            self.legs[i].set_target_angles(&angles[i])
        }
    }

    pub fn write_angles(&self) 
    {
        let mut file = File::create("/tmp/hexsim/leg_output").unwrap();

        let mut result = String::from("");
        for leg in &self.legs
        {
            for angle in leg.get_angles()
            {
                result += format!("{},", angle).as_str();
            }
            result += "\n";
        }

        file.write_all(result.into_bytes().as_slice());
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


    result
}

fn main() {
    let mut window = Window::new("Kiss3d: cube");

    window.set_light(Light::StickToCamera);

    let mut robot = Robot::new(&mut window);

    let mut old_time = time::precise_time_s() as f32;
    while window.render() 
    {
        let new_time = time::precise_time_s() as f32;
        let delta_time = new_time - old_time;
        old_time = new_time;

        let target_angles = read_target_angles();

        robot.set_target_angles(target_angles);
        robot.update(delta_time);
        robot.write_angles();
    }
}
