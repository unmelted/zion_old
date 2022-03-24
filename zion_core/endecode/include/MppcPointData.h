////////////////////////////////////////////
// Multi Position Preset Calibration Data //
////////////////////////////////////////////

#pragma once

#include <map>

struct MppcWorldCoords
{
    double X1{};
    double Y1{};
    double X2{};
    double Y2{};
    double X3{};
    double Y3{};
    double X4{};
    double Y4{};
};

struct MppcWorlds
{
    std::string group;
    std::string stadium;
    MppcWorldCoords world_coords;
};

struct MppcPoint
{
    bool IsEmpty{};
    double X{};
    double Y{};
};

struct MppcPts2D
{
    double UpperPosX{};
    double UpperPosY{};
    double MiddlePosX{};
    double MiddlePosY{};
    double LowerPosX{};
    double LowerPosY{};
    MppcPoint Upper;
    MppcPoint Middle;
    MppcPoint Lower;
};

struct MppcPts3D
{
    double X1{};
    double Y1{};
    double X2{};
    double Y2{};
    double X3{};
    double Y3{};
    double X4{};
    double Y4{};
    double CenterX{};
    double CenterY{};
    MppcPoint Point1;
    MppcPoint Point2;
    MppcPoint Point3;
    MppcPoint Point4;
    MppcPoint Center;
};

struct MppcPtsSwipe
{
    double X1{};
    double Y1{};
    double X2{};
    double Y2{};
};

struct MppcPoints
{
    int point_index{};
    std::string dsc_id;
    int framenum{};
    int camfps{};
    std::string presd_path; // PreSD file name of this point.
    std::vector<uint8_t> bitstream; // Frame Data
    int flip{};
    std::string Group;
    int Width{};
    int Height{};
    int infection_point{};
    double swipe_base_length{};
    int ManualOffsetY{};
    double FocalLength{};
    int recalibration_status{}; //RETURNED - Status : #0: no run yet. #-1 : not found in range #1: found success
    double recalibration_score{};  //RETURNED -  Score : 0 - 100 score of found point
    bool mppc{}; // true: This instance is for MPPC. / false: This instance is not for MPPC. (Note: Do not modify this value.)

    MppcPts2D pts_2d;
    MppcPts3D pts_3d;
    MppcPtsSwipe pts_swipe;
    MppcWorldCoords world_coords; // Note: It's for the use of ESMMovieMaker only. the 'VideoMaker' instance manages it, so don't save it to the preset file.
};

struct MppcPositionPreset
{
    std::string RecordName;
    std::map<std::string, MppcWorlds> worlds; // Key:group
    std::map<std::string, MppcPoints> points; // Key:DscID
};

/// <remarks>
/// MMd manages the 'currentPresetNumber' value. So, please do not modify it arbitrarily.
/// </remarks>
struct MppcPointData
{
    int currentPresetNumber{}; // If 'preset' map is empty or no preset is selected, this value is 0.
    std::map<int, MppcPositionPreset> preset; // Key:Preset Number (1, 2, ...) (Note: There is no preset whose number is 0.)
};