
-----------------------------------------------------------------------------------------------------------------------
project "radosgw-admin"
  language "C++"
  kind "ConsoleApp"

  files { "*.cpp", "*.hpp" }

  libdirs { "../../boost/stage/lib" }
  includedirs { "../../boost", "../../" }

  links { "oberon" }

  targetdir( "../../builds/bin")

  configuration { "gmake" }
    linkoptions { "-static -pthread" }
    buildoptions { "-std=c++11" }

    links       { "boost_chrono",
                  "boost_date_time",
                  "boost_filesystem",
                  "boost_regex",
                  "boost_program_options",
                  "boost_system" }

  configuration "Debug"
       defines { "DEBUG" }
       flags { "Symbols" }

  configuration "Release"
      defines { "NDEBUG" }
      flags { "Optimize" }


-----------------------------------------------------------------------------------------------------------------------
