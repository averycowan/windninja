/******************************************************************************
 *
 * $Id$
 *
 * Project:  WindNinja
 * Purpose:  Class that stores all inputs to run WindNinja
 * Author:   Jason Forthofer <jforthofer@gmail.com>
 *
 ******************************************************************************
 *
 * THIS SOFTWARE WAS DEVELOPED AT THE ROCKY MOUNTAIN RESEARCH STATION (RMRS)
 * MISSOULA FIRE SCIENCES LABORATORY BY EMPLOYEES OF THE FEDERAL GOVERNMENT
 * IN THE COURSE OF THEIR OFFICIAL DUTIES. PURSUANT TO TITLE 17 SECTION 105
 * OF THE UNITED STATES CODE, THIS SOFTWARE IS NOT SUBJECT TO COPYRIGHT
 * PROTECTION AND IS IN THE PUBLIC DOMAIN. RMRS MISSOULA FIRE SCIENCES
 * LABORATORY ASSUMES NO RESPONSIBILITY WHATSOEVER FOR ITS USE BY OTHER
 * PARTIES,  AND MAKES NO GUARANTEES, EXPRESSED OR IMPLIED, ABOUT ITS QUALITY,
 * RELIABILITY, OR ANY OTHER CHARACTERISTIC.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 *****************************************************************************/
#ifndef WINDNINJAINPUTS_H
#define WINDNINJAINPUTS_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <iomanip>
#include <string.h>
#include <string>
#include <exception>

#include "SurfProperties.h"
#include "Elevation.h"
#include "ninjaUnits.h"
#include "KmlVector.h"
#include "wxStation.h"
#include "ninjaCom.h"
#include "ninja_conv.h"
/*
#ifdef WINDNINJA_EXPORTS
    #define WINDNINJA_API __declspec(dllexport) 	
#else
    #define WINDNINJA_API
#endif
*/

struct WindNinjaInputs
{
public:
    WindNinjaInputs();
    ~WindNinjaInputs();

    WindNinjaInputs(const WindNinjaInputs &rhs);
    WindNinjaInputs &operator=(const WindNinjaInputs &rhs);


    /*-----------------------------------------------------------------------------
     *  Enumerated types
     *-----------------------------------------------------------------------------*/
    enum eVegetation{
        grass,
        brush,
        trees
    };

    enum eOutputType{
        mesh,
        google,
        shapefile,
        ASCII
    };

    enum eInitializationMethod{
        noInitializationFlag,			//no initialization, used to check if it has been set or not
        domainAverageInitializationFlag,	//single domain-averaged input speed and direction
        pointInitializationFlag,		//single or multiple point inititialization
        wxModelInitializationFlag	//Weather forecast model initialization
    };

    eVegetation vegetation;

    /*-----------------------------------------------------------------------------
     *  Base input data passed to "simulate_wind()"
     *-----------------------------------------------------------------------------*/

    
    ninjaComClass *Com;	//pointer to a com handler for the specific communication type desired
    char lastComString[NINJA_MSG_SIZE];
    int inputsRunNumber;
    ninjaComClass::eNinjaCom inputsComType;


    //DEM input
    Elevation dem;				//elevation data in gridded format, HORIZONTAL UNITS MUST ALWAYS BE IN METERS, INCLUDING WHEN IT IS READ IN FROM A FILE!

    /*-----------------------------------------------------------------------------
     *  Input speed/direction Parameters
     *-----------------------------------------------------------------------------*/
    eInitializationMethod initializationMethod;	//method to initialize WindNinja
    std::string forecastFilename;	//name of coarse weather model initialization file (NDFD, NAM, GFS, RUC, etc.)
    velocityUnits::eVelocityUnits inputSpeedUnits;			//units of input windspeed (0=>m/s, 1=>mph, 2=>kph) (note that inputSpeed is always stored as m/s, and converted to and from the other units)
    velocityUnits::eVelocityUnits outputSpeedUnits;			//units of output windspeed (0=>m/s, 1=>mph, 2=>kph)
    double inputSpeed;			//input wind speed in m/s
    double inputDirection;		//input wind direction (measured in degrees clockwise from North; DIRECTION WIND COMES FROM)
    lengthUnits::eLengthUnits inputWindHeightUnits;	//units of inputWindHeight when read in (always stored in meters!)
    double inputWindHeight;		//height of input wind above the top of the vegetation (always stored in meters!)
    lengthUnits::eLengthUnits outputWindHeightUnits;	//units of outputWindHeight when read in (always stored in meters!)
    double outputWindHeight;		//height of output wind above the top of the vegetation (always stored in meters!)
    std::vector<wxStation> stations;		//array of weather stations used in point initialization
    std::string wxStationFilename;	//filename of a weather station(s) file
    std::vector<wxStation> stationsScratch;		//scratch space for a copy of WindNinjaInputs::stations to use during outer ninja interations for wind field vs wx station matching
    bool matchWxStations;		//flag used to determine if wx stations matching should be done
    double outer_relax;			//relax/ation value (Lopes found 0.8 to be good compromise between fast convergence and to prevent divergence)

    
    /*-----------------------------------------------------------------------------
     *  Surface Properties
     *-----------------------------------------------------------------------------*/
    surfProperties surface;		//surface properties data (roughness, albedo, etc...)

    /*-----------------------------------------------------------------------------
     *  Stability Properties
     *-----------------------------------------------------------------------------*/
    //bool stabilityFlag;  //flag specifying if non-neutral stability parameters should be set

    /*-----------------------------------------------------------------------------
     *  Diurnal Inputs
     *-----------------------------------------------------------------------------*/
    bool diurnalWinds;			//flag that enables or disables diurnal wind computation (true=>include diurnal winds,  false=>don't include diurnal winds)
    boost::local_time::local_date_time ninjaTime;		//time and date class
    boost::local_time::time_zone_ptr ninjaTimeZone;		//timezone information
    boost::local_time::tz_database tz_db;			//timezone database read from file
    temperatureUnits::eTempUnits airTempUnits;		//units of air temperature, used in diurnal computations
    double airTemp;			//air temperature used
    coverUnits::eCoverUnits cloudCoverUnits;		//units of cloud cover (0=>fraction, 1=>percent) ALWAYS STORED AS FRACTION!
    double cloudCover;			//fraction of cloud cover (range is 0-1, NOT PERCENT!)

    double latitude;			//approximate latitude of modeling area (used in diurnal calcs for solar angle) ALWAYS STORED AS DECIMAL DEGREES!
    double longitude;			//approximate longitude of modeling area (used in diurnal calcs for solar angle) ALWAYS STORED AS DECIMAL DEGREES!

    /*-----------------------------------------------------------------------------
     *  CPU Paremeters
     *-----------------------------------------------------------------------------*/
    int numberCPUs;			//number of CPUs to use (at this point, only the diurnal is multithreaded...)

    
    /*-----------------------------------------------------------------------------
     *  Output Parameters
     *-----------------------------------------------------------------------------*/
    double outputBufferClipping;	//specifies the percent to clip the output files around the perimeter.  Range 0-50.
    bool writeAtmFile;          //flage specifying if a Farsite .atm file should be written.
    bool googOutFlag;			//flag specifying if a Google Earth file (*.kmz) should be written (this can only be done if the DEM has an associated *.prj file)
    KmlVector::egoogSpeedScaling googSpeedScaling;		//flag specifying the speed scaling for the legend/colors in the *.kmz file (0=>equal colors, 1=>equal interval)
    double googLineWidth;		//drawing line width for google output vectors
    bool wxModelGoogOutFlag;			//flag specifying if a Google Earth file (*.kmz) should be written (this can only be done if the DEM has an associated *.prj file)
    KmlVector::egoogSpeedScaling wxModelGoogSpeedScaling;		//flag specifying the speed scaling for the legend/colors in the *.kmz file (0=>equal colors, 1=>equal interval)
    double wxModelGoogLineWidth;		//drawing line width for google output vectors
    bool shpOutFlag;			//flag specifying if a shapefile (*.shp, *.shx, *.dbf) should be written
    bool asciiOutFlag;			//flag specifying if ESRI Ascii Raster files (*_vel.asc, *_ang.asc, *_cld.asc) should be written
    bool txtOutFlag;			//flag specifying if a text file (*.txt) comparing measured to simulated data at specified points should be written (filenames here are hard-coded into the write_compare_output() function in ninja.cpp)
    bool wxModelShpOutFlag;		//flag specifying if a wxModel shapefile should be written
    bool wxModelAsciiOutFlag;		//flag specifying if wxModel ESRI Ascii Raster files should be written
    bool volVTKOutFlag;			//flag specifying if a volume VTK file should be written
    std::string kmlFile;
    std::string kmzFile;
    std::string wxModelKmlFile;
    std::string wxModelKmzFile;
    double kmzResolution;
    lengthUnits::eLengthUnits kmzUnits;
    std::string shpFile;
    std::string dbfFile;
    std::string wxModelShpFile;
    std::string wxModelDbfFile;
    double shpResolution;
    lengthUnits::eLengthUnits shpUnits;
    std::string cldFile;
    std::string wxModelCldFile;
    std::string velFile;
    std::string wxModelVelFile;
    double velResolution;
    lengthUnits::eLengthUnits velOutputFileDistanceUnits;				//distance units of resolution
    std::string angFile;
    std::string atmFile;
    std::string wxModelAngFile;
    double angResolution;
    lengthUnits::eLengthUnits angOutputFileDistanceUnits;				//distance units of resolution
    std::string legFile;
    std::string dateTimeLegFile;
    std::string wxModelLegFile;
    std::string dateTimewxModelLegFile;
    std::string volVTKFile;
    bool        pdfOutFlag;
    std::string pdfDEMFileName;
    std::string pdfFile;
    double      pdfResolution;
    lengthUnits::eLengthUnits pdfUnits;

    /*-----------------------------------------------------------------------------
     *  Ninja Speed Testing Section
     *-----------------------------------------------------------------------------*/
#ifdef NINJA_SPEED_TESTING
    double speedDampeningRatio;  //initialization speed dampening ratio for wx model runs
#endif
    double downDragCoeff; // downslope drag coefficient for diurnal calculations
    double downEntrainmentCoeff; // downslope entrainment coefficient for diurnal calculations
    double upDragCoeff; // upslope drag coefficient for diurnal calculations
    double upEntrainmentCoeff; // upslope entrainment coefficient for diurnal calculations 
    
    /*-----------------------------------------------------------------------------
     *  Friction Velocity Section
     *-----------------------------------------------------------------------------*/
#ifdef FRICTION_VELOCITY
    bool frictionVelocityFlag;  //flag specifying if friction velocity should be computed
    std::string frictionVelocityCalculationMethod; //options are logProfile and shearStress
    std::string ustarFile;
#endif

    /*-----------------------------------------------------------------------------
     *  EMISSIONS Section
     *-----------------------------------------------------------------------------*/
#ifdef EMISSIONS
    bool dustFlag;  //flag specifying if dust emissions should be computed
    std::string dustFileOut;   //filename of the output dust emissions file
    std::string dustFilename;   //filename of fire perimeter for dust emissions calculation
    std::string dustFile;
    std::string geotiffOutFilename; //filename of multiband geotiff output file
    bool geotiffOutFlag; //flag specifying if multiband geotiff output should be written
#endif

    /*-----------------------------------------------------------------------------
     *  STABILITY section
     *-----------------------------------------------------------------------------*/
#ifdef STABILITY
    double alphaStability;
    bool stabilityFlag;  //flag specifying if non-neutral stability parameters should be set
#endif

    /*-----------------------------------------------------------------------------
     *  SCALAR section
     *-----------------------------------------------------------------------------*/
#ifdef SCALAR
    bool scalarTransportFlag;  //flag specifiying if scalar transport should be calculated
    double scalarSourceStrength;
    double scalarSourceXcoord;  //input long
    double scalarSourceYcoord;  //input lat
#endif //SCALAR

    std::string outputPointsFilename; //name of file containing output for requested point locations
    std::string inputPointsFilename; // name of file containing locations of specfic points for output
    
    bool keepOutGridsInMemory; //flag to determine if the final grids should be kept in memory after simulate_wind() or not.  Normally this is done only for a dll run.

    std::string outputPath;
    
    std::vector<std::string> pointsNamesList; //vector of input point locations names
    std::vector<double> latList; //vectors of input point locations in lat/lon
    std::vector<double> lonList;
    std::vector<double> heightList;
    std::vector<double> projYList; //vectors of input point locations in proj coords
    std::vector<double> projXList;

    /*-----------------------------------------------------------------------------
     *  NinjaFOAM section
     *-----------------------------------------------------------------------------*/
#ifdef NINJAFOAM
    int nIterations; //number of iterations for a ninjafoam simulation
    int meshCount; //mesh count for a ninjafoam simulation
    std::string meshCoice; // fine, medium, coarse
    bool nonEqBc; //flag indicating if non-equilbrium boundary conditions should be used for a ninjafoam simulation

#endif
};

#endif	/* WINDNINJAINPUTS_H */
