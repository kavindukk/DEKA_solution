#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <math.h>
#include <memory>

#pragma region DTOs
/// <summary>
/// Simple structure to store a 3D vector (representing coordinates, directions, etc.)
/// We are assuming a left-handed coordinate system with X-right, Y-up, and Z-forward (same as Unity's coordinate axes)
/// NOTE: You can extend this definition in any way you want.
/// </summary>
struct Vector3
{
    float X;
    float Y;
    float Z;
    Vector3(float x, float y, float z) : X(x), Y(y), Z(z)
    {
    }
    float norm()
    {
        return sqrt(X*X + Y*Y + Z*Z);
    }

    Vector3 operator+(const Vector3& other) const 
    {
        return Vector3(X + other.X, Y + other.Y, Z + other.Z);
    }

    Vector3 operator-(const Vector3& other) const 
    {
        return Vector3(X - other.X, Y - other.Y, Z - other.Z);
    }

    Vector3 operator*(const float& scalar) const 
    {
        return Vector3(X * scalar, Y * scalar, Z * scalar);
    }
    Vector3 operator/(const float& scalar) const 
    {
        return Vector3(X / scalar, Y / scalar, Z / scalar);
    }


    std::string ToString() const
    {
        return "(X: " + std::to_string(X) + ", Y: " + std::to_string(Y) + ", Z: " + std::to_string(Z) + ")";
    }
    
    // Feel free to add to the "Vector3" definition below...
};

float dotProduct(const Vector3& vec1, const Vector3& vec2)  
{
    return vec1.X*vec2.X + vec1.Y*vec2.Y + vec1.Z*vec2.Z;
}


/// <summary>
/// Simple structure to store the concept of our simplified One World Trade Center building, which is defined by the vertices of both its base footprint and its top face
/// (which are both squares, and are rotated 45 degrees relative to each other).
/// </summary>
struct Building
{
    /// <summary>
    /// The four vertices of the square base of the building (in clockwise order)
    /// </summary>
    std::vector<Vector3> BaseCorners;
    
    /// <summary>
    /// The four vertices of the square top face of the building (in clockwise order), which is offset by a 45 degree clockwise rotation from the base.
	/// NOTE: You can therefore assume the first element of the TopCorners list is the vertex that lies above the edge defined by the first and second elements of the BaseCorners list.
	/// (In other words BaseCorners[0], BaseCorners[1], and TopCorners[0] form one of the perfeclty flat, triangular vertical faces described in the supplemental material)
	/// In other words 
    /// </summary>
    std::vector<Vector3> TopCorners;
	
	/// <summary>
    /// Building constructor
    /// </summary>
    Building(std::vector<Vector3> baseCorners, std::vector<Vector3> topCorners) : BaseCorners(baseCorners), TopCorners(topCorners)
    {
        if (baseCorners.size() != 4 || topCorners.size() != 4)
        {
            throw;
        }
    }
    
    // Feel free to add to the "Building" definition below...
};
#pragma endregion DTOs

#pragma region Do Not Modify
const std::string Tab = "\t";
const std::string NewLine = "\n";

void PrintLine(std::string s)
{
    std::cout << s << NewLine; 
}

// std::string GetVectorString(const std::vector<Vector3>& vec)
// {
//     if (vec.size() == 0)
//     {
//         return "";
//     }
   
//     std::string delimiter = NewLine + Tab;
//     std::string output = delimiter + vec[0].ToString();
//     for (int i=1; i < 4; i++)
//     {
//         output += delimiter + vec[i].ToString();
//     }
//     return output;
// };
std::string GetVectorString(const std::vector<Vector3>& vec)
{
    if (vec.size() == 0)
    {
        return "";
    }
   
    std::string delimiter = NewLine + Tab;
    std::string output = delimiter + vec[0].ToString();
    for (int i=1; i < vec.size(); i++)
    {
        output += delimiter + vec[i].ToString();
    }
    return output;
};

/// <summary>
/// Retrieve a Building object that adheres to the requirements of our simplified One World Trade Center geometry
/// (which includes a square base and a square top face, which is rotated 45 degrees relative to the base).
/// NOTE: You should NOT edit the below function
/// </summary>
/// <returns></returns>
Building GetRandomBuilding()
{
    srand(time(0));
    float baseMin = 10, baseMax = 30, heightMin = 100, heightMax = 200;
    
    auto GetRandomInRange = [](float min, float max) -> float
    {
        return min + static_cast <float> (rand()) / ( static_cast <float> (RAND_MAX / (max-min))); 
    };
    
    
    auto GetNormalized = [](const Vector3& original) -> Vector3
    {
        return original * (1 / (float) sqrt(pow(original.X, 2) + pow(original.Z, 2)));
    };
    
    auto GetPerpendicular2D = [&](const Vector3& original) -> Vector3 
    {
        return GetNormalized(Vector3(original.Z, 0, -original.X));
        
    };

    auto GetRandomHorizontalUnitVector = [&]() -> Vector3
    {
        float theta = GetRandomInRange(0, (float) M_PI * 2);
        return Vector3((float) cos(theta), 0, (float) sin(theta));
    };

    // Generate random properties of building, including its height, base foot print size, vertex coordinates, etc.
    float baseSideLength = GetRandomInRange(baseMin, baseMax);
    float baseDiagonalLength = baseSideLength * sqrt(2);
    float height = GetRandomInRange(heightMin, heightMax);
    Vector3 heightVector = Vector3(0, height, 0);
    Vector3 forward = GetRandomHorizontalUnitVector();
    Vector3 right = GetPerpendicular2D(forward);
    Vector3 diagonalPoint = GetNormalized(forward + right) * (baseDiagonalLength / 2);

    // Determine the corner vertices of the building (based on the above properties) by "walking around" both the base and top face
    // starting at some random point headed in some random direction
    Vector3 currentPoint = diagonalPoint;
    Vector3 currentDirection = forward * -1;
    std::vector<Vector3> basePoints;
    std::vector<Vector3> topPoints;
    for (int i = 0; i < 4; i++)
    {
        basePoints.push_back(currentPoint);
        topPoints.push_back(basePoints[i] + currentDirection * (baseSideLength / 2) + heightVector);
        currentPoint = currentPoint + currentDirection * baseSideLength;
        currentDirection = GetPerpendicular2D(currentDirection);
    }
    
    return Building(basePoints, topPoints);
}
#pragma endregion Do Not Modify