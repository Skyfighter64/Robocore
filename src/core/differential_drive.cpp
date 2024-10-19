#include "motor_driver.cpp"
#include "rotary_encoder.cpp"
#include "vector2d.cpp"
#include <limits>

/**
 * This class serves as differential drive kinematic and inverse kinematic,
 * calculating the robots position from its sensors and letting it drive 
 * based on positional and rotational inputs
*/

// define mathematical constants
#define PI_FOURTH  0.7854
#define TWO_PI 6.2832

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
     * Convert a directional angle to wheel speed percentages according
     * to the inverse kinematics of a differential drive robot
     * 
     * angle: the requested driving angle for the differential drive robot
     * 
     * returns:
     *  Vector2D(left, right): a vector containing the left and right wheel's
     *  driving percentages for the given angle
    */
    Vector2D DifferentialDriveInverseKinematics(double angle)
    {
        double left_percentage;
        double right_percentage;

        if(angle <= 1 * PI_FOURTH)
        {
            left_percentage = tan(-angle + PI_FOURTH);
            right_percentage = 1;
        }
        else if (angle <= 2 * PI_FOURTH)
        {
            left_percentage = tan(angle + PI_FOURTH);
            right_percentage = 1;
        }
        else if (angle <= 3 * PI_FOURTH)
        {
            left_percentage = -1;
            right_percentage = tan(3*PI_FOURTH - angle);
        }
        else if (angle <= 4 * PI_FOURTH)
        {
            left_percentage = -1;
            right_percentage = tan(angle - PI_FOURTH);
        }
        else if (angle <= 5 * PI_FOURTH)
        {
            left_percentage = tan(5*PI_FOURTH - angle);     
            right_percentage = -1;
        }
        else if (angle <= 6 * PI_FOURTH)
        {
            left_percentage = tan(angle - 3*PI_FOURTH);     
            right_percentage = -1;                 
        }
        else if (angle <= 7 * PI_FOURTH)
        {
            left_percentage = 1;     
            right_percentage = tan(7*PI_FOURTH - angle); 
        }
         else if (angle <= 8 * PI_FOURTH)
        {
            left_percentage = 1;
            right_percentage = tan(angle - 5*PI_FOURTH);     
        }
        return Vector2D(left_percentage,right_percentage);
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
     *
     *  0 (0°) | pi * 8/4 (360°) : forward (no rotation)
     *
     * Left Rotations:
     *  pi*1/4 (45°): turn left only using the right wheel
     *  pi*2/4 (90°): turn left on the spot
     *  pi*3/4 (135°): turn left only using the left wheel (backwards)
     * 
     *  pi     (180°): reverse (no rotation)
     * 
     * Right rotations:  
     *  pi*5/4 (225°): turn right only using the left wheel
     *  pi*6/4 (270°): turn right on the spot
     *  pi*7/4 (315°): turn right only using the right wheel (backwards)
     *  
     * 
    */
    void Drive(int speed, double angle)
    {
        // normalize angle to be within 0 - 2*pi
        angle = angle - (int(angle/TWO_PI)*TWO_PI);
        // calculate the motor speeds according to the angle
        Vector2D inverseKinematics = DifferentialDriveInverseKinematics(angle);
        // apply the speed to the results
        int left_speed = inverseKinematics.x * speed;
        int right_speed = inverseKinematics.y * speed;
        // set the speed to the motors
        leftMotor.SetSpeed(left_speed);
        rightMotor.SetSpeed(right_speed);
    }

    void Stop()
    {
        leftMotor.Stop();
        rightMotor.Stop();
    }

};