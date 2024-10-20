#include <core/vector2d.cpp>
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

void test_operator_equal()
{
    TEST_ASSERT_EQUAL(true, Vector2D(1,4) == Vector2D(1,4));
    TEST_ASSERT_EQUAL(true, Vector2D(0,0) == Vector2D(0,0));
    TEST_ASSERT_EQUAL(true, Vector2D(-3,5) == Vector2D(-3,5));
    TEST_ASSERT_EQUAL(false, Vector2D(-3,5) == Vector2D(-2,6));
}

void test_operator_unequal()
{
    TEST_ASSERT_TRUE( Vector2D(1,4) != Vector2D(4,1));
    TEST_ASSERT_TRUE( Vector2D(0,0) != Vector2D(-2,2));
    TEST_ASSERT_FALSE(Vector2D(-3,5) != Vector2D(-3,5));
}

void test_operator_add()
{
    TEST_ASSERT_TRUE(Vector2D(3,4) == Vector2D(1,2) + Vector2D(2,2));
}

void test_operator_subtract()
{
    TEST_ASSERT_TRUE(Vector2D(-1,0) == Vector2D(1,2) - Vector2D(2,2));
}

int runUnityTests(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_operator_equal);   
    RUN_TEST(test_operator_unequal);    
    RUN_TEST(test_operator_add);
    RUN_TEST(test_operator_subtract);

    return UNITY_END();
}

/**
  * For Arduino framework
  */
void setup() {
  // Wait ~2 seconds before the Unity test runner
  // establishes connection with a board Serial interface
  delay(2000);

  runUnityTests();
}
void loop() {}
