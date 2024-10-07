add_rules("mode.debug", "mode.release")

add_requires("raylib")

-- imgui settings
local imgui_dir = path.join("lib/imgui-docking/", "imgui")
local imgui_files = {
    path.join(imgui_dir, "imgui.cpp"),
    path.join(imgui_dir, "imgui_draw.cpp"),
    path.join(imgui_dir, "imgui_widgets.cpp"),
    path.join(imgui_dir, "imgui_tables.cpp"),
    path.join(imgui_dir, "imgui_demo.cpp"),
}

-- rlImgui settings
local rlimgui_dir = "lib/rlImGui/"
local rlimgui_files = {
    path.join(rlimgui_dir, "rlImGui.cpp"),
}

-- Common and Core directories settings
local common_dir = "src/Common/"
local common_files = {
    path.join(common_dir, "*.cpp"),
}

local core_dir = "src/Core/"
local core_files = {
    path.join(core_dir, "*.cpp"),
}

target("Pseudo Hilbert Curve")
    set_kind("binary")
    set_languages("c++20")

    add_includedirs(imgui_dir, rlimgui_dir, common_dir, core_dir)
    add_files(table.unpack(imgui_files))
    add_files(table.unpack(rlimgui_files))


    add_files("src/*.cpp") 

    add_packages("raylib")

--
-- If you want to known more usage about xmake, please see https://xmake.io
--
-- ## FAQ
--
-- You can enter the project directory firstly before building project.
--
--   $ cd projectdir
--
-- 1. How to build project?
--
--   $ xmake
--
-- 2. How to configure project?
--
--   $ xmake f -p [macosx|linux|iphoneos ..] -a [x86_64|i386|arm64 ..] -m [debug|release]
--
-- 3. Where is the build output directory?
--
--   The default output directory is `./build` and you can configure the output directory.
--
--   $ xmake f -o outputdir
--   $ xmake
--
-- 4. How to run and debug target after building project?
--
--   $ xmake run [targetname]
--   $ xmake run -d [targetname]
--
-- 5. How to install target to the system directory or other output directory?
--
--   $ xmake install
--   $ xmake install -o installdir
--
-- 6. Add some frequently-used compilation flags in xmake.lua
--
-- @code
--    -- add debug and release modes
--    add_rules("mode.debug", "mode.release")
--
--    -- add macro definition
--    add_defines("NDEBUG", "_GNU_SOURCE=1")
--
--    -- set warning all as error
--    set_warnings("all", "error")
--
--    -- set language: c99, c++11
--    set_languages("c99", "c++11")
--
--    -- set optimization: none, faster, fastest, smallest
--    set_optimize("fastest")
--
--    -- add include search directories
--    add_includedirs("/usr/include", "/usr/local/include")
--
--    -- add link libraries and search directories
--    add_links("tbox")
--    add_linkdirs("/usr/local/lib", "/usr/lib")
--
--    -- add system link libraries
--    add_syslinks("z", "pthread")
--
--    -- add compilation and link flags
--    add_cxflags("-stdnolib", "-fno-strict-aliasing")
--    add_ldflags("-L/usr/local/lib", "-lpthread", {force = true})
--
-- @endcode
--

