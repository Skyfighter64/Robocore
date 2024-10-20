#include <core/differential_drive.cpp>
#include <unity.h>
#include <Arduino.h>

void setUp(void)
{
  // set stuff up here
}

void tearDown(void)
{
  // clean stuff up here
}

void test_function_DifferentialDriveInverseKinematics()
{
    double left;
    double right;
    std::tie(left,right) = DifferentialDrive::DifferentialDriveInverseKinematics(0);
    TEST_ASSERT_DOUBLE_WITHIN(0.001, 1, left);
    TEST_ASSERT_DOUBLE_WITHIN(0.001, 1, right);

    std::tie(left,right) = DifferentialDrive::DifferentialDriveInverseKinematics(PI/4);
    TEST_ASSERT_DOUBLE_WITHIN(0.001, 0, left);
    TEST_ASSERT_DOUBLE_WITHIN(0.001, 1, right);

    std::tie(left,right) = DifferentialDrive::DifferentialDriveInverseKinematics(PI/2.0);
    TEST_ASSERT_DOUBLE_WITHIN(0.001, -1, left);
    TEST_ASSERT_DOUBLE_WITHIN(0.001, 1, right);

    std::tie(left,right) = DifferentialDrive::DifferentialDriveInverseKinematics(3*PI/4);
    TEST_ASSERT_DOUBLE_WITHIN(0.001, -1, left);
    TEST_ASSERT_DOUBLE_WITHIN(0.001, 0, right);

    std::tie(left,right) = DifferentialDrive::DifferentialDriveInverseKinematics(PI);
    TEST_ASSERT_DOUBLE_WITHIN(0.001, -1, left);
    TEST_ASSERT_DOUBLE_WITHIN(0.001, -1,  right);

    std::tie(left,right) = DifferentialDrive::DifferentialDriveInverseKinematics(5*PI/4);
    TEST_ASSERT_DOUBLE_WITHIN(0.001, 0, left);
    TEST_ASSERT_DOUBLE_WITHIN(0.001, -1,  right);

    std::tie(left,right) = DifferentialDrive::DifferentialDriveInverseKinematics(6*PI/4);
    TEST_ASSERT_DOUBLE_WITHIN(0.001, 1, left);
    TEST_ASSERT_DOUBLE_WITHIN(0.001, -1, right);

    std::tie(left,right) = DifferentialDrive::DifferentialDriveInverseKinematics(7*PI/4);
    TEST_ASSERT_DOUBLE_WITHIN(0.001, 1, left);
    TEST_ASSERT_DOUBLE_WITHIN(0.001, 0, right);
}

void test_function_NormalizeAngle()
{
  TEST_ASSERT_DOUBLE_WITHIN(0.001, 0, DifferentialDrive::NormalizeAngle(0));
  TEST_ASSERT_DOUBLE_WITHIN(0.001, 0, DifferentialDrive::NormalizeAngle(TWO_PI));
  TEST_ASSERT_DOUBLE_WITHIN(0.001, 0, DifferentialDrive::NormalizeAngle(2*TWO_PI));
  TEST_ASSERT_DOUBLE_WITHIN(0.001, 0, DifferentialDrive::NormalizeAngle(-2*TWO_PI));

  TEST_ASSERT_DOUBLE_WITHIN(0.001, PI, DifferentialDrive::NormalizeAngle(PI));
  TEST_ASSERT_DOUBLE_WITHIN(0.001, 0.1528, DifferentialDrive::NormalizeAngle(0.1528));
  TEST_ASSERT_DOUBLE_WITHIN(0.001, 0.1528, DifferentialDrive::NormalizeAngle(TWO_PI + 0.1528));
  TEST_ASSERT_DOUBLE_WITHIN(0.001, TWO_PI - 0.1528, DifferentialDrive::NormalizeAngle(- 0.1528));

  TEST_ASSERT_DOUBLE_WITHIN(0.001, 0.1528, DifferentialDrive::NormalizeAngle(- TWO_PI + 0.1528));

}

int runUnityTests(void)
{
    UNITY_BEGIN();
    
    RUN_TEST(test_function_NormalizeAngle); 
    RUN_TEST(test_function_DifferentialDriveInverseKinematics);   

    return UNITY_END();
}


  //For Arduino framework
 
void setup() {
  // Wait ~2 seconds before the Unity test runner
  // establishes connection with a board Serial interface
  delay(2000);

  runUnityTests();
}
void loop() {}

