/**
 * Class representing 2-dimensional Vectors
*/
class Vector2D
{
public: 
    // x and y values of this vector in mm
    int x;
    int y; 
    Vector2D() : x(0), y(0) {}
    Vector2D(int _x, int _y) : x(_x), y(_y) {}

    // overload arithmetric operators
    Vector2D operator+(Vector2D other)
    {
        return Vector2D(this->x + other.x, this->y + other.y); 
    }
    Vector2D operator-(Vector2D other)
    {
        return Vector2D(this->x - other.x, this->y - other.y);  
    }
    bool operator==(Vector2D other)
    {
        return (this->x == other.x && this->y == other.y);
    }
    bool operator!=(Vector2D other)
    {
        return !(*this == other);
    }

};