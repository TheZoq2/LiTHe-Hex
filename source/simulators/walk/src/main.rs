extern crate kiss3d;
extern crate nalgebra as na;

use na::Vector3;
use kiss3d::window::Window;
use kiss3d::light::Light;
use kiss3d::scene::SceneNode;

use std::f32::consts::PI;

use std::path::Path;

struct Leg
{
    nodes: (SceneNode, SceneNode, SceneNode),
    angles: (f32, f32, f32)
}

impl Leg
{
    pub fn new(window: &mut Window) -> Leg
    {
        //let mut limb1 = window.add_cube(0.2, 1.0, 0.2);
        //let mut limb2 = limb1.add_cube(0.2, 1.0, 0.2);
        //let mut limb3 = limb2.add_cube(0.2, 1.0, 0.2);
        let mut limb1 = window.add_obj(
                    &Path::new("media/cube.obj"),
                    &Path::new("media/cube.mtl"),
                    Vector3::new(1., 1.,1.)
                );
        let mut limb2 = limb1.add_obj(
                    &Path::new("media/cube.obj"),
                    &Path::new("media/cube.mtl"),
                    Vector3::new(1., 1.,1.)
                );
        let mut limb3 = limb2.add_obj(
                    &Path::new("media/cube.obj"),
                    &Path::new("media/cube.mtl"),
                    Vector3::new(1., 2.,1.)
                );

        limb1.set_color(1., 0., 0.);
        limb2.set_color(0., 1., 0.);
        limb3.set_color(0., 0., 1.);

        limb2.set_local_translation(Vector3::new(0.0, 1.0, 0.0));
        limb3.set_local_translation(Vector3::new(0.0, 1.0, 0.0));

        limb1.set_local_rotation(Vector3::new(PI/2., 0., 0.));
        limb2.set_local_rotation(Vector3::new(-PI/3., 0., 0.));
        limb3.set_local_rotation(Vector3::new(2. * PI/3., 0., 0.));

        Leg {
            nodes: (limb1, limb2, limb3),
            angles: (PI/2., 0., 0.)
        }
    }

    pub fn update(&mut self)
    {
        self.nodes.0.prepend_to_local_rotation(&Vector3::new(0., 0., 0.014));
    }
}

fn main() {
    let mut window = Window::new("Kiss3d: cube");

    window.set_light(Light::StickToCamera);

    let mut test_leg = Leg::new(&mut window);

    while window.render() {
        //c.prepend_to_local_rotation(&Vector3::new(0.0, 0., 0.014));
        //c2.prepend_to_local_rotation(&Vector3::new(0.0, 0.014, 0.0));
        test_leg.update();
    }
}
