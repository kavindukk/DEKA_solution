/******************************************************************************
Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, Java, PHP, Ruby, Perl,
C#, VB, Swift, Pascal, Fortran, Haskell, Objective-C, Assembly, HTML, CSS, JS, SQLite, Prolog.
Code, Compile, Run and Debug online from anywhere in world.
*******************************************************************************/
#include "helper.h"
#define PI 3.14159265358979323846 /*pi*/

#pragma region Your Answers
float GetHeight(const Building& building)
{    
    float height  = building.TopCorners[0].Y;
    return height;
}

//My function to calculate lenght between two 3D points
float GetLengthBetweenTwoPoints(const Vector3& vec1, const Vector3& vec2)
{
    Vector3 substractVec = vec1 - vec2;
    float x = substractVec.X;
    float y  = substractVec.Y;
    float z = substractVec.Z;
    float length = sqrt(x*x + y*y + z*z);
    return length;
}

float GetBaseArea(const Building& building)
{
    std::vector<Vector3> points = building.BaseCorners;
    Vector3 point0=points[0];
    Vector3 point1=points[1];
    float length = GetLengthBetweenTwoPoints(point0,point1);
    return length*length;
}

/// <summary>
/// Return the angle (in radians) that each slanted face of the building forms with the ground (don't worry about the sign).
/// By "slanted face" we mean any of the triangular faces that appear to extend diagonally down from the top face and have their apex at a corner vertex of the base.
/// They are colored light-grey and dark-grey in the 3D render included in the supplementary material.
/// </summary>
float GetSlantedFaceAngle(const Building& building)
{
    /*
    According to given vertex diagram, Top point 0 and bottom point 1(one slanted vertex) should be adequate to calculate
    the slant angle from ground.
    That slant angle should be equal to other 3. 
    */
    Vector3 top0=building.TopCorners[0], base1=building.BaseCorners[1];
    Vector3 xzPlane = Vector3(0.,1.,0.);
    float cosValueBetweenxzPlaneAndTop0Base1Line =dotProduct((top0-base1),xzPlane)/xzPlane.norm()/(top0-base1).norm(); // cos(alpha) = x.y/(norm(x)*norm(y))
    float angleBetweenxzPlaneAndTop0Base1Line = acos(cosValueBetweenxzPlaneAndTop0Base1Line); // angle from y axis
    return  PI/2 - angleBetweenxzPlaneAndTop0Base1Line; // angle from x axis
}

/// <summary>
/// Return a list of points that define the 'middle floor' of the building.
/// These can be thought of as the corners (or hull) of the shape cut out by a horizontal plane intersecting the building at Y = BuildingHeight / 2.
/// </summary>
Vector3 getMidPointOfTwoVectors(const Vector3& p1,const Vector3& p2)
{
    return (p1 + p2)/2.0;
}

std::vector<Vector3> GetMiddleFloorCorners(const Building& building)
{ 
    std::vector<Vector3> middleFloorPoints;

    std::vector<Vector3> topPoints = building.TopCorners;
    std::vector<Vector3> bottomPoints = building.BaseCorners;

    std::vector<int> bottomPointIndexes = {0, 1, 2, 3};
    std::vector<std::vector<int>> topPointIndexes = {{3,0},{0,1},{1,2},{2,3}};
    for (int bottomIndex:bottomPointIndexes)
    {
        for(int topIndex: topPointIndexes[bottomIndex])
        {
            Vector3 topPoint = topPoints[topIndex];
            Vector3 bottomPoint = bottomPoints[bottomIndex];
            Vector3 midPoint = getMidPointOfTwoVectors(topPoint, bottomPoint);
            middleFloorPoints.push_back(midPoint);            
        }
    }
    
    return middleFloorPoints;
}

/// <summary>
/// Calculate the area of the middle floor of the building.
/// This can be thought of as the area of the shape cut out by a horizontal plane intersecting the building at Y = BuildingHeight / 2.
/// BONUS POINTS: If you can write to the console the algebraic relationship between this area (M) and the area of the base (B).
/// HINT: Remember, lengths will change linearly, but areas won't
/// </summary>
float GetAreaOfMiddleFloor(const Building& building, std::string& bonusDescription)
{
    
    Vector3 point1 = GetMiddleFloorCorners(building)[0];
    point1.Y = 0;
    Vector3 point2 = building.BaseCorners[0];
    float length = GetLengthBetweenTwoPoints(point1,point2);
    float reducableArea = 2*length*length;
    float middleFloorArea = GetBaseArea(building) - reducableArea;
    bonusDescription = "BONUS: The relationship between area of middle floor (M) and the area of the base (B) is:"
                        "There are four identical triangles, which should be removed from the base area for it to be equal with the middle floor area. "
                       "Those 4 triangles make two identical squares, and the length of an edge of the square is the horizontal length between the respective "
                       "bottom point to the middle floor point.";
    return middleFloorArea;
}

/// <summary>
/// Calculate the volume of the building.
/// BONUS POINTS: If you can write to the console the algebraic relationship between this volume (V) and the AREA of the base (B).
/// </summary>
float GetVolume(const Building& building, std::string& bonusDescription)
{
    Vector3 point1 = building.BaseCorners[0];
    Vector3 point2 = building.TopCorners[3];
    float height = point2.Y;
    point2.Y = 0;
    float length = GetLengthBetweenTwoPoints(point1, point2);    
    float reducableVol = length*length*height;
    float volumeOfBuilding = GetBaseArea(building)*height - reducableVol;

    bonusDescription = "BONUS: The relationship between the volume of the building (V) and the area of the base (B) is: The volume of the outer cube and"
                       " the inner cube can be calculated using the given coordinates of the building. if the difference between two cubes is divided into"
                       " two and then subtract from the outer cube or added to the inner cube should give the volume of the given shape. Four subtracted "
                       "shapes from the outer cube should form a column which's height is the building height and the length and width are equal to the "
                       "horizontal length between top 0 and bottom 0.";
    return volumeOfBuilding;
}
#pragma endregion Your Answers

#pragma No need to modify below    
int main()
{
    Building building = GetRandomBuilding();

    float baseArea = GetBaseArea(building);
    float height = GetHeight(building);
    float slantAngle = GetSlantedFaceAngle(building);
    std::vector<Vector3> middleFloorCorners = GetMiddleFloorCorners(building);
    std::string areaBonus;
    float middleFloorArea = GetAreaOfMiddleFloor(building, areaBonus);
    std::string volumeBonus;
    float volume = GetVolume(building, volumeBonus);
    
    PrintLine("Base Corners: " + GetVectorString(building.BaseCorners));
    PrintLine("Top Corners: " + GetVectorString(building.TopCorners));
    PrintLine("Area of Base: " + std::to_string(baseArea));
    PrintLine("Height: " + std::to_string(height));
    PrintLine("Angle (radians) of slanted face(s): " + std::to_string(slantAngle));
    PrintLine("Middle Floor Corners: " + GetVectorString(middleFloorCorners));
    PrintLine("Area of Middle Floor: " + std::to_string(middleFloorArea));
    PrintLine(areaBonus);
    PrintLine("Volume: " + std::to_string(volume));
    PrintLine(volumeBonus);

    return 0;
}
#pragma endregion