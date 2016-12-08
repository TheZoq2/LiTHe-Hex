extern crate kiss3d;
extern crate nalgebra as na;
extern crate time;
extern crate rustc_serialize;

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

use rustc_serialize::json;

use na::{Vector3, Point3};

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
                    na::Vector3::new(10., 0., -6.) * constants::UNIT_SCALE, //Left front
                    na::Vector3::new(10., 0., 6.) * constants::UNIT_SCALE, //Right front
                    na::Vector3::new(0., 0., -8.) * constants::UNIT_SCALE, //Left mid
                    na::Vector3::new(0., 0., 10.) * constants::UNIT_SCALE, //Right mid
                    na::Vector3::new(-10., 0., -6.) * constants::UNIT_SCALE, //Left back
                    na::Vector3::new(-10., 0., 6.) * constants::UNIT_SCALE, //Right back
                );

        let leg_angles = vec!(PI/4., -PI/4., PI/2., -PI/2., 3. * PI/4., -3. * PI/4.);

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

    pub fn set_targets(&mut self, targets: Vec<LegTarget>)
    {
        assert_eq!(targets.len(), self.legs.len());

        for i in 0..self.legs.len()
        {
            self.legs[i].set_target_angles(&targets[i].angles);
            self.legs[i].set_target_point(&targets[i].point);
        }
    }

    pub fn write_angles_done(&self) 
    {
        let mut file = File::create("/tmp/hexsim/servo_states").unwrap();

        let mut result = String::from("");
        for leg in &self.legs
        {
            for status in leg.is_still_moving()
            {
                result += match status
                {
                    true => "1",
                    false => "0"
                }
            }
        }

        file.write_all(result.into_bytes().as_slice());
    }
}

#[derive(RustcDecodable)]
struct LegTarget
{
    point: na::Vector3<f32>,
    angles: Vec<f32>
}
impl LegTarget
{
    pub fn convert_to_simulator_coordinates(&self) -> LegTarget
    {
        const scaling: f32 = 10. * constants::UNIT_SCALE;

        let mut new_target = LegTarget{
            angles: self.angles.clone(), 
            point: na::zero()
        };

        new_target.point.x = self.point.x * scaling;
        new_target.point.y = self.point.z * scaling;
        new_target.point.z = -self.point.y * scaling;

        new_target
    }
}

//Panics if file is wrong, maybe change?
fn read_target_angles() -> Vec<LegTarget>
{
    let file_dir = String::from("/tmp/hexsim");
    let filepath = file_dir.clone() + "/leg_input";

    try_with_yolo!(std::fs::create_dir(&file_dir));

    //Opening the file
    //let mut file = None;
    let mut is_done = false;
    let mut result: Vec<LegTarget> = vec!();
    
    while !is_done
    {
        let mut file = match File::open(&filepath)
        {
            Ok(val) => val,
            Err(e) => {
                println!("Failed to open target file {}, {:?}", filepath, e);
                continue;
            }
        };

        //Reading the file into a string
        let mut s = String::new();
        file.read_to_string(&mut s).unwrap();

        //let json_data = json::Json::from_str(&s).unwrap();

        result = match json::decode(&s)
        {
            Ok(val) => {
                is_done = true;
                val
            },
            Err(e) => {
                println!("Failed to parse json, {}", e);
                continue;
            }
        };
    }

    result = result.into_iter().map(|x|{x.convert_to_simulator_coordinates()}).collect();

    result
}

fn draw_grid(window: &mut Window)
{
    for pos in -1000..1000
    {
        let pos = pos as f32;
        if pos != 0.
        {
            window.draw_line(&Point3::new(pos, 0., -1000.), &Point3::new(pos, 0., 1000.), &Point3::new(0.5, 0.5, 0.5));
            window.draw_line(&Point3::new(-1000., 0., pos), &Point3::new(1000., 0., pos), &Point3::new(0.5, 0.5, 0.5));
        }
    }
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

        let leg_targets = read_target_angles();

        robot.set_targets(leg_targets);
        robot.update(delta_time);
        robot.write_angles_done();

        draw_grid(&mut window);

        window.draw_line(&Point3::new(0., 0., 0.), &Point3::new(1000., 0., 0.), &Point3::new(1., 0., 0.));
        window.draw_line(&Point3::new(0., 0., 0.), &Point3::new(0., 1000., 0.), &Point3::new(0., 1., 0.));
        window.draw_line(&Point3::new(0., 0., 0.), &Point3::new(0., 0., 1000.), &Point3::new(0., 0., 1.));
    }
}
