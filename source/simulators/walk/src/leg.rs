extern crate kiss3d;
extern crate nalgebra as na;
extern crate time;

use na::Vector3;
use kiss3d::window::Window;
use kiss3d::scene::SceneNode;

use std::f32::consts::PI;

use std::path::Path;
use std::vec::Vec;

const LIMB1_LENGTH: f32 = 5.5 / 4.;
const LIMB2_LENGTH: f32 = 6.5 / 4.;
const LIMB3_LENGTH: f32 = 13.5 / 4.;

pub struct Limb
{
    pub target_angle: f32,
    pub turn_speed: f32,

    node: SceneNode,
    base_rotation: Vector3<f32>,
    current_angle: f32,
    turn_axis: Vector3<f32>,
}

impl Limb
{
    pub fn new(
                node: SceneNode,
                base_rotation: Vector3<f32>,
                turn_speed: f32,
                turn_axis: Vector3<f32>
              ) 
            -> Limb 
    {
        Limb 
        {
            base_rotation: base_rotation,

            node: node,
            target_angle: 0.,
            turn_speed: turn_speed,
            current_angle: 0.,
            turn_axis: turn_axis
        }
    }

    pub fn update(&mut self, delta_time: f32) 
    {
        let angle_diff = self.target_angle - self.current_angle;

        if angle_diff.abs() < self.turn_speed * delta_time
        {
            self.current_angle = self.target_angle;
        }
        else if angle_diff > 0.
        {
            self.current_angle += self.turn_speed * delta_time;
        }
        else
        {
            self.current_angle -= self.turn_speed * delta_time;
        }

        self.node.set_local_rotation(
                    self.base_rotation + self.turn_axis * self.current_angle);
    }

    pub fn get_angle(&self) -> f32 
    {
        self.current_angle
    }
}

pub struct Leg
{
    limbs: Vec<Limb>,
}

impl Leg
{
    pub fn new(window: &mut Window, angle: f32, position: Vector3<f32>) -> Leg
    {
        let mut leg_group = window.add_group();
        leg_group.set_local_rotation(Vector3::new(0., angle, 0.));
        leg_group.set_local_translation(position);

        let mut node1 = leg_group.add_obj(
                    &Path::new("media/cube.obj"),
                    &Path::new("media/cube.mtl"),
                    Vector3::new(1., LIMB1_LENGTH,1.)
                );
        let mut node2 = node1.add_obj(
                    &Path::new("media/cube.obj"),
                    &Path::new("media/cube.mtl"),
                    Vector3::new(1., LIMB2_LENGTH,1.)
                );
        let mut node3 = node2.add_obj(
                    &Path::new("media/cube.obj"),
                    &Path::new("media/cube.mtl"),
                    Vector3::new(1., LIMB3_LENGTH,1.)
                );

        node1.set_color(1., 0., 0.);
        node2.set_color(0., 1., 0.);
        node3.set_color(0., 0., 1.);

        node2.set_local_translation(Vector3::new(0.0, LIMB1_LENGTH, 0.0));
        node3.set_local_translation(Vector3::new(0.0, LIMB2_LENGTH, 0.0));

        let turn_speed = 0.5;
        let limb1 = Limb::new(
                                node1, 
                                Vector3::new(PI/2., 0., 0.), 
                                turn_speed, 
                                Vector3::new(0.,0.,1.)
                            );
        let limb2 = Limb::new(
                                node2, 
                                Vector3::new(-PI/3., 0., 0.), 
                                turn_speed, 
                                Vector3::new(1.,0.,0.)
                            );
        let limb3 = Limb::new(
                                node3, 
                                Vector3::new(2. * PI/3., 0., 0.), 
                                turn_speed, 
                                Vector3::new(1.,0.,0.)
                            );

        Leg {
            limbs: vec!(limb1, limb2, limb3),
        }
    }

    pub fn update(&mut self, delta_time: f32)
    {
        for limb in &mut self.limbs
        {
            limb.update(delta_time);
        }
    }

    pub fn set_target_angles(&mut self, angles: &Vec<f32>)
    {
        assert_eq!(angles.len(), self.limbs.len());

        for i in 0..angles.len()
        {
            self.limbs[i].target_angle = angles[i].clone();
        }
    }

    pub fn get_angles(&self) -> Vec<f32>
    {
        let mut result = vec!();
        for limb in &self.limbs
        {
            result.push(limb.get_angle());
        }

        result
    }
}
