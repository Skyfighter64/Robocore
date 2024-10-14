#include "motor_driver.cpp"
#include "rotary_encoder.cpp"
#include "vector2d.cpp"
#include <limits>

/**
 * This class serves as differential drive kinematic and inverse kinematic,
 * calculating the robots position from its sensors and letting it drive 
 * based on positional and rotational inputs
*/

// define motor driver pin numbers
#define LEFT_FORWARD 19
#define LEFT_REVERSE 21
#define RIGHT_FORWARD 22
#define RIGHT_REVERSE 23

//define rotary encoder pin numbers
#define LEFT_ENCODER 3
#define RIGHT_ENCODER 4
// encoder specific parameters
#define ENCODER_STEPS 36
#define HISTORY_SIZE 5

#define WHEEL_CIRCUMFENCE_MM 314
#define WHEEL_DISTANCE_MM 270


class DifferentialDrive
{

 // this class should be specific to this robot
 // -> use rotary encoders to calculate odometry
 // -> use robot measurements (wheel circumfence, wheel distance) 
 // -> use motor drivers to run the motors 
 // Tasks:
 // - calculate and track current robot odometry position
 // - provide current speed, position to outside 
 //     - get position
 //     - get speed vector
 //     - get rotation 
 // - provide functions for driving the robot
 //    - drive based on a goal position
 //    - turn for certain angle, drive for certain distance 

private:
    // declare left and right wheel motor drivers
    MotorDriver leftMotor = MotorDriver(LEFT_FORWARD, LEFT_REVERSE);
    MotorDriver rightMotor = MotorDriver(RIGHT_FORWARD, RIGHT_REVERSE);

    // declare left and right wheel rotation sensors
    //RotaryEncoder leftEncoder = RotaryEncoder(LEFT_ENCODER, ENCODER_STEPS, HISTORY_SIZE);
    //RotaryEncoder rightEncoder = RotaryEncoder(RIGHT_ENCODER, ENCODER_STEPS, HISTORY_SIZE);

    // Robot position and rotation
    Vector2D position = Vector2D(0,0);
    // note: all angles are in radiants
    float rotation = 0;

public:
    /**
     * Run this function periodically to update the current position and 
     * Robot driving instructions.
     * 
     * This needs to be called as much as possible so the rotary encoders 
     * do not miss a step. Todo: use interrupts or second core for this or rtos multitasking
     */
    void Update()
    {
        // read rotary encoders and update position
        //int leftDelta = leftEncoder.getCountDelta() * leftMotor.GetDirection();
        //int rightDelta = rightEncoder.getCountDelta() * rightMotor.GetDirection();

        // calculate movement from delta
        //position += KinematicTranslation(leftDelta, rightDelta);
        //rotation += KinematicRotation(leftDelta, rightDelta);

        // update motor speeds

        // measure time from last encoder read
        // calculate wheel speeds
        // adjust wheel speeds using pid? or just try percentages first (left 100%, right percentage for this)
        // find out when goal distance was reached
        // todo
    }

    /**
     * get the robots position in the odometry frame
    */
    Vector2D GetPosition()
    {
        return position;
    }

    /**
     * Get the robot rotation relative to the odometry frame X-axis
     * 
    */
    float GetRotation()
    {
        return rotation;
    }

    /**
     * Drive forward with the given speed
     * 
    */
    void DriveForward(int speed)
    {
        DriveCircle(std::numeric_limits<float>::infinity(), speed);
    }
    /**
     * Drive on a circular trajectory with the given radius for the 
     * given distance
     * radius: the radius of the circle trajectory in mm. 
     *   Positive: Right-handed rotation
     *   Negative: Left-handed rotation
     *   std::numeric_limits<float>::infinity() for driving straight
     *  
     * speed: driving speed from -255 to 255
     * 
    */
    void DriveCircle(float radius, int speed)
    {
        // calculate speed of both wheels
        int leftSpeed;
        int rightSpeed;

        if (radius == std::numeric_limits<float>::infinity())
        {
            // drive forward if radius is infinite
            leftSpeed = speed;
            rightSpeed = speed;
        }
        else if (radius == WHEEL_DISTANCE_MM / 2.0)
        {
            // right side full power, left side 0
            leftSpeed = 0;
            rightSpeed = speed;
        }
        else if (radius == - (WHEEL_DISTANCE_MM / 2.0))
        {
            // left side full power, right side 0
            leftSpeed = speed;
            rightSpeed = 0;
        }
        else if(radius >= 0)
        {
            // left side rotation
            // right side full speed, left side less by the ratio depending on rotation direction
            double ratio = (radius - (WHEEL_DISTANCE_MM / 2.0)) / (radius + (WHEEL_DISTANCE_MM / 2.0));
            rightSpeed = speed;
            leftSpeed = int(speed * ratio);
        }
        else if (radius < 0)
        {
            // right side rotation
            // right side full speed, left side less by the ratio depending on rotation direction
            double ratio = (radius + (WHEEL_DISTANCE_MM / 2.0)) / (radius - (WHEEL_DISTANCE_MM / 2.0));
            rightSpeed = int(speed * ratio);
            leftSpeed = speed;
        }

        leftMotor.SetSpeed(leftSpeed);
        rightMotor.SetSpeed(rightSpeed);
        
    }
    /**
     * Drive with the given speed in the given direction
     * speed: the speed to drive with. speed of the fastest wheel
     * -255: full speed reverse
     *    0: stopped
     *  255: full speed
     * 
     * angle: the angle in radians to rotate while driving; right-handed and towards the robot x-axis (pointing forward) 
     * Notable Angles (edge cases):
     *  0 (0°) : forward (no rotation)
     * Left Rotations:
     *  pi*1/4 (45°): turn left only using the right wheel
     *  pi*2/4 (90°): turn left on the spot
     *  pi*3/4 (135°): turn left only using the left wheel (backwards)
     * 
     *  pi     (180°): reverse (no rotation)
     * 
     * Right rotations:  
     *  -pi*1/4 (-45°): turn right only using the left wheel
     *  -pi*2/4 (-90°): turn right on the spot
     *  -pi*3/4 (-135°): turn right only using the right wheel (backwards)
     * 
     *  std::numeric_limits<float>::infinity() for driving straight
     * 
    */
    void Drive(int speed, float angle)
    {
        //todo
    }

    void Stop()
    {
        leftMotor.Stop();
        rightMotor.Stop();
    }

};