extern crate kiss3d;
extern crate nalgebra as na;
extern crate time;

use na::Vector3;
use kiss3d::window::Window;
use kiss3d::scene::SceneNode;

use std::f32::consts::PI;

use std::path::Path;
use std::vec::Vec;

use constants::UNIT_SCALE;
use constants;


const LIMB1_LENGTH: f32 = 5.5 * UNIT_SCALE;
const LIMB2_LENGTH: f32 = 6.5 * UNIT_SCALE;
const LIMB3_LENGTH: f32 = 13.5 * UNIT_SCALE;

const JOINT2_ANGLE_OFFSET: f32 = 0.349;
const JOINT3_ANGLE_OFFSET: f32 = 0.873;


#[derive(Clone)]
pub struct Limb
{
    pub target_angle: f32,
    pub turn_speed: f32,

    node: SceneNode, //The node that contains the legs
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

        //if angle_diff.abs() < self.turn_speed * delta_time
        //{
        //    self.current_angle = self.target_angle;
        //}
        //else if angle_diff > 0.
        //{
        //    self.current_angle += self.turn_speed * delta_time;
        //}
        //else
        //{
        //    self.current_angle -= self.turn_speed * delta_time;
        //}

        const scaling: f32 = 0.1;

        self.current_angle += angle_diff * scaling;


        self.node.set_local_rotation(
                    self.base_rotation + self.turn_axis * self.current_angle);
    }

    pub fn get_angle(&self) -> f32 
    {
        self.current_angle
    }

    pub fn is_done_rotating(&self) -> bool
    {
        self.current_angle == self.target_angle
    }

    pub fn get_target_angle(&self) -> f32
    {
        self.target_angle
    }
}

pub struct Leg
{
    limbs: Vec<Limb>,
    position: Vector3<f32>,

    //Keeping track and visualising the current target
    target_node: SceneNode,
    target_point: Vector3<f32>,
}

impl Leg
{
    pub fn new(window: &mut Window, angle: f32, position: Vector3<f32>) -> Leg
    {
        let mut leg_group = window.add_group();
        leg_group.set_local_rotation(Vector3::new(0., angle - PI/2., 0.));
        leg_group.set_local_translation(position);

        //Group that rotates the legs into the propper plane
        let mut inner_leg_group = leg_group.add_group();
        inner_leg_group.set_local_rotation(Vector3::new(-PI/2., 0., 0.));

        let mut node1 = inner_leg_group.add_obj(
                    &Path::new("media/cube.obj"),
                    &Path::new("media/cube.mtl"),
                    Vector3::new(1., LIMB1_LENGTH,1.)
                );
        let mut node2 = node1.add_obj(
                    &Path::new("media/cube.obj"),
                    &Path::new("media/cube.mtl"),
                    Vector3::new(1., LIMB2_LENGTH / LIMB1_LENGTH, 1.)
                );
        let mut node3 = node2.add_obj(
                    &Path::new("media/cube.obj"),
                    &Path::new("media/cube.mtl"),
                    Vector3::new(1., LIMB3_LENGTH / (LIMB2_LENGTH),1.)
                );

        node1.set_color(1., 0., 0.);
        node2.set_color(0., 1., 0.);
        node3.set_color(0., 0., 1.);

        println!("{}", LIMB2_LENGTH);

        node2.set_local_translation(Vector3::new(0.0, LIMB1_LENGTH, 0.0));
        node3.set_local_translation(Vector3::new(0.0, LIMB2_LENGTH, 0.0));

        let turn_speed = constants::TURN_SPEED;
        let limb1 = Limb::new(
                                node1, 
                                Vector3::new(0., 0., 0.), 
                                turn_speed, 
                                Vector3::new(0.,0.,-1.)
                            );
        let limb2 = Limb::new(
                                node2, 
                                Vector3::new(-JOINT2_ANGLE_OFFSET, 0., 0.), 
                                turn_speed, 
                                Vector3::new(1.,0.,0.)
                            );
        let limb3 = Limb::new(
                                node3, 
                                Vector3::new(-JOINT3_ANGLE_OFFSET, 0., 0.), 
                                turn_speed, 
                                Vector3::new(1.,0.,0.)
                            );

        //Node for visualising the legs current target
        let target_node = window.add_sphere(0.1);

        Leg {
            limbs: vec!(limb1, limb2, limb3),
            position: position,
            target_node: target_node,
            target_point: na::zero(),
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

    pub fn set_target_point(&mut self, target: &Vector3<f32>)
    {
        self.target_point = *target;

        self.target_node.set_local_translation(*target / UNIT_SCALE + self.position);
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

    pub fn is_still_moving(&self) -> Vec<bool>
    {
        self.limbs.clone().into_iter().map(|x|{x.is_done_rotating() == false}).collect()
    }

    pub fn get_target_angles(&self) -> Vec<f32>
    {
        self.limbs.clone().into_iter().map(|x|{x.get_target_angle()}).collect()
    }
}
