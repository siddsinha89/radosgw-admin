solution "Solution"
    location("builds")
    configurations { "Debug", "Release" }
    
-----------------------------------------------------------------------------------------------------------------------
project "oberon"
    language "C++"
    kind "StaticLib" 
    
    files { "oberon/*.cpp", "oberon/*.hpp" } 
    
    libdirs { "boost/stage/lib" }
              
    includedirs { "boost", "." }
     
    targetdir( "builds/oberon")

    configuration "Debug"
        defines { "DEBUG" }
        flags { "Symbols" }

    configuration "Release"
        defines { "NDEBUG" }
        flags { "Optimize" }

    configuration "gmake"
         buildoptions { "-std=c++11" }

         links { "boost_chrono",
                 "boost_date_time", 
                 "boost_filesystem", 
                 "boost_program_options", 
                 "boost_system" }
                 
    -- boost auto-linking takes care of our links specification on windows

-----------------------------------------------------------------------------------------------------------------------

include "application/radosgw-admin"

