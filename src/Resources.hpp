#include "ResourceId.hpp"

#if defined(USE_GDI) || defined(ANDROID)

#define MAKE_RESOURCE(name, id) \
  static constexpr ResourceId name(id);

#else

#include "Util/ConstBuffer.hpp"

#define MAKE_RESOURCE(name, id) \
  extern const char resource_ ## id[]; \
  extern const char resource_ ## id ## _size[]; \
  static constexpr ResourceId name(resource_ ##id, resource_ ##id ## _size);

#endif

MAKE_RESOURCE(IDB_LANDABLE, 139);
MAKE_RESOURCE(IDB_LANDABLE_HD, 5139);
MAKE_RESOURCE(IDB_REACHABLE, 140);
MAKE_RESOURCE(IDB_REACHABLE_HD, 5140);
MAKE_RESOURCE(IDB_TURNPOINT, 141);
MAKE_RESOURCE(IDB_TURNPOINT_HD, 5141);
MAKE_RESOURCE(IDB_SMALL, 142);
MAKE_RESOURCE(IDB_SMALL_HD, 5142);
MAKE_RESOURCE(IDB_CRUISE, 212);
MAKE_RESOURCE(IDB_CRUISE_HD, 5212);
MAKE_RESOURCE(IDB_CLIMB, 213);
MAKE_RESOURCE(IDB_CLIMB_HD, 5213);
MAKE_RESOURCE(IDB_FINALGLIDE, 214);
MAKE_RESOURCE(IDB_FINALGLIDE_HD, 5214);
MAKE_RESOURCE(IDB_TOWN, 218);
MAKE_RESOURCE(IDB_TOWN_HD, 5218);
MAKE_RESOURCE(IDB_MARK, 219);
MAKE_RESOURCE(IDB_MARK_HD, 5219);
MAKE_RESOURCE(IDB_TERRAINWARNING, 220);
MAKE_RESOURCE(IDB_TERRAINWARNING_HD, 5220);
MAKE_RESOURCE(IDB_LOGO_HD, 251);
MAKE_RESOURCE(IDB_GPSSTATUS1, 256);
MAKE_RESOURCE(IDB_GPSSTATUS1_HD, 5256);
MAKE_RESOURCE(IDB_GPSSTATUS2, 257);
MAKE_RESOURCE(IDB_GPSSTATUS2_HD, 5257);
MAKE_RESOURCE(IDB_ABORT, 269);
MAKE_RESOURCE(IDB_ABORT_HD, 5269);
MAKE_RESOURCE(IDB_LOGGER, 271);
MAKE_RESOURCE(IDB_LOGGER_HD, 5271);
MAKE_RESOURCE(IDB_LOGGEROFF, 272);
MAKE_RESOURCE(IDB_LOGGEROFF_HD, 5272);
MAKE_RESOURCE(IDB_UNIT_KM, 284);
MAKE_RESOURCE(IDB_UNIT_SM, 285);
MAKE_RESOURCE(IDB_UNIT_NM, 286);
MAKE_RESOURCE(IDB_UNIT_M, 287);
MAKE_RESOURCE(IDB_UNIT_FT, 288);
MAKE_RESOURCE(IDB_AIRPORT_REACHABLE, 289);
MAKE_RESOURCE(IDB_AIRPORT_REACHABLE_HD, 5289);
MAKE_RESOURCE(IDB_AIRPORT_UNREACHABLE, 290);
MAKE_RESOURCE(IDB_AIRPORT_UNREACHABLE_HD, 5290);
MAKE_RESOURCE(IDB_OUTFIELD_REACHABLE, 291);
MAKE_RESOURCE(IDB_OUTFIELD_REACHABLE_HD, 5291);
MAKE_RESOURCE(IDB_OUTFIELD_UNREACHABLE, 292);
MAKE_RESOURCE(IDB_OUTFIELD_UNREACHABLE_HD, 5292);
MAKE_RESOURCE(IDB_VARIOSCALEA, 293);
MAKE_RESOURCE(IDB_UNIT_MS, 294);
MAKE_RESOURCE(IDB_UNIT_KT, 295);
MAKE_RESOURCE(IDB_UNIT_FL, 296);
MAKE_RESOURCE(IDB_UNIT_DegK, 297);
MAKE_RESOURCE(IDB_UNIT_DegC, 298);
MAKE_RESOURCE(IDB_UNIT_DegF, 299);
MAKE_RESOURCE(IDB_UNIT_FPM, 300);
MAKE_RESOURCE(IDB_UNIT_KMH, 301);
MAKE_RESOURCE(IDB_UNIT_MPH, 302);
MAKE_RESOURCE(IDB_CLIMBSMALL, 313);
MAKE_RESOURCE(IDB_CLIMBSMALLINV, 314);
MAKE_RESOURCE(IDB_VARIOSCALEC, 315);
MAKE_RESOURCE(IDB_THERMALSOURCE, 316);
MAKE_RESOURCE(IDB_THERMALSOURCE_HD, 5316);
MAKE_RESOURCE(IDB_TARGET, 320);
MAKE_RESOURCE(IDB_TARGET_HD, 5320);
MAKE_RESOURCE(IDB_TEAMMATE_POS, 322);
MAKE_RESOURCE(IDB_TEAMMATE_POS_HD, 5322);
MAKE_RESOURCE(IDB_LAUNCHER1, 330);
MAKE_RESOURCE(IDB_LAUNCHER2, 331);
MAKE_RESOURCE(IDB_LOGO, 332);
MAKE_RESOURCE(IDB_AIRPORT_UNREACHABLE2, 334);
MAKE_RESOURCE(IDB_AIRPORT_UNREACHABLE2_HD, 5334);
MAKE_RESOURCE(IDB_OUTFIELD_UNREACHABLE2, 335);
MAKE_RESOURCE(IDB_OUTFIELD_UNREACHABLE2_HD, 5335);
MAKE_RESOURCE(IDB_AIRSPACEI, 336);
MAKE_RESOURCE(IDB_AIRSPACEI_HD, 5336);
MAKE_RESOURCE(IDB_PROGRESSBORDER, 337);
MAKE_RESOURCE(IDB_TITLE, 338);
MAKE_RESOURCE(IDB_TITLE_HD, 339);
MAKE_RESOURCE(IDB_MAPSCALE_LEFT, 340);
MAKE_RESOURCE(IDB_MAPSCALE_RIGHT, 341);
MAKE_RESOURCE(IDB_MAPSCALE_LEFT_HD, 5340);
MAKE_RESOURCE(IDB_MAPSCALE_RIGHT_HD, 5341);

#ifdef EYE_CANDY
MAKE_RESOURCE(IDB_DIALOGTITLE, 342);
#endif

MAKE_RESOURCE(IDB_MOUNTAIN_TOP, 343);
MAKE_RESOURCE(IDB_MOUNTAIN_TOP_HD, 5343);
MAKE_RESOURCE(IDB_BRIDGE, 344);
MAKE_RESOURCE(IDB_BRIDGE_HD, 5344);
MAKE_RESOURCE(IDB_TUNNEL, 345);
MAKE_RESOURCE(IDB_TUNNEL_HD, 5345);
MAKE_RESOURCE(IDB_TOWER, 346);
MAKE_RESOURCE(IDB_TOWER_HD, 5346);
MAKE_RESOURCE(IDB_POWER_PLANT, 347);
MAKE_RESOURCE(IDB_POWER_PLANT_HD, 5347);
MAKE_RESOURCE(IDB_FOLDER, 348);
MAKE_RESOURCE(IDB_FOLDER_HD, 5348);
MAKE_RESOURCE(IDB_SETTINGS, 349);
MAKE_RESOURCE(IDB_SETTINGS_HD, 5349);
MAKE_RESOURCE(IDB_TASK, 350);
MAKE_RESOURCE(IDB_TASK_HD, 5350);
MAKE_RESOURCE(IDB_CALCULATOR, 351);
MAKE_RESOURCE(IDB_CALCULATOR_HD, 5351);
MAKE_RESOURCE(IDB_AIRPORT_MARGINAL, 352);
MAKE_RESOURCE(IDB_AIRPORT_MARGINAL_HD, 5352);
MAKE_RESOURCE(IDB_OUTFIELD_MARGINAL, 353);
MAKE_RESOURCE(IDB_OUTFIELD_MARGINAL_HD, 5353);
MAKE_RESOURCE(IDB_AIRPORT_MARGINAL2, 354);
MAKE_RESOURCE(IDB_AIRPORT_MARGINAL2_HD, 5354);
MAKE_RESOURCE(IDB_OUTFIELD_MARGINAL2, 355);
MAKE_RESOURCE(IDB_OUTFIELD_MARGINAL2_HD, 5355);
MAKE_RESOURCE(IDB_MARGINAL, 356);
MAKE_RESOURCE(IDB_MARGINAL_HD, 5356);
MAKE_RESOURCE(IDB_WRENCH, 357);
MAKE_RESOURCE(IDB_WRENCH_HD, 5357);
MAKE_RESOURCE(IDB_GLOBE, 358);
MAKE_RESOURCE(IDB_GLOBE_HD, 5358);
MAKE_RESOURCE(IDB_DEVICE, 359);
MAKE_RESOURCE(IDB_DEVICE_HD, 5359);
MAKE_RESOURCE(IDB_RULES, 360);
MAKE_RESOURCE(IDB_RULES_HD, 5360);
MAKE_RESOURCE(IDB_CLOCK, 361);
MAKE_RESOURCE(IDB_CLOCK_HD, 5361);
MAKE_RESOURCE(IDB_TRAFFIC_SAFE, 362);
MAKE_RESOURCE(IDB_TRAFFIC_SAFE_HD, 5362);
MAKE_RESOURCE(IDB_TRAFFIC_WARNING, 363);
MAKE_RESOURCE(IDB_TRAFFIC_WARNING_HD, 5363);
MAKE_RESOURCE(IDB_TRAFFIC_ALARM, 364);
MAKE_RESOURCE(IDB_TRAFFIC_ALARM_HD, 5364);
MAKE_RESOURCE(IDB_TASKTURNPOINT, 365);
MAKE_RESOURCE(IDB_TASKTURNPOINT_HD, 5365);
MAKE_RESOURCE(IDB_OBSTACLE, 366);
MAKE_RESOURCE(IDB_OBSTACLE_HD, 5366);
MAKE_RESOURCE(IDB_MOUNTAIN_PASS, 367);
MAKE_RESOURCE(IDB_MOUNTAIN_PASS_HD, 5367);
MAKE_RESOURCE(IDB_WEATHER_STATION, 368);
MAKE_RESOURCE(IDB_WEATHER_STATION_HD, 5368);
MAKE_RESOURCE(IDB_THERMAL_HOTSPOT, 369);
MAKE_RESOURCE(IDB_THERMAL_HOTSPOT_HD, 5369);
MAKE_RESOURCE(IDB_MENU_BUTTON, 500);
MAKE_RESOURCE(IDB_MENU_BUTTON_HD, 5500);
MAKE_RESOURCE(IDB_ZOOM_OUT_BUTTON, 501);
MAKE_RESOURCE(IDB_ZOOM_OUT_BUTTON_HD, 5501);
MAKE_RESOURCE(IDB_ZOOM_IN_BUTTON, 502);
MAKE_RESOURCE(IDB_ZOOM_IN_BUTTON_HD, 5502);
MAKE_RESOURCE(IDB_CHECK_MARK, 503);
MAKE_RESOURCE(IDB_CHECK_MARK_HD, 5503);
MAKE_RESOURCE(IDB_SEARCH, 504);
MAKE_RESOURCE(IDB_SEARCH_HD, 5504);
MAKE_RESOURCE(IDB_SEARCH_CHECKED, 505);
MAKE_RESOURCE(IDB_SEARCH_CHECKED_HD, 5505);
MAKE_RESOURCE(IDB_BEARING_LEFT_ONE, 506);
MAKE_RESOURCE(IDB_BEARING_LEFT_ONE_HD, 5506);
MAKE_RESOURCE(IDB_BEARING_LEFT_TWO, 507);
MAKE_RESOURCE(IDB_BEARING_LEFT_TWO_HD, 5507);
MAKE_RESOURCE(IDB_BEARING_LEFT_THREE, 508);
MAKE_RESOURCE(IDB_BEARING_LEFT_THREE_HD, 5508);
MAKE_RESOURCE(IDB_BEARING_LEFT_FOUR, 509);
MAKE_RESOURCE(IDB_BEARING_LEFT_FOUR_HD, 5509);
MAKE_RESOURCE(IDB_BEARING_RIGHT_ONE, 510);
MAKE_RESOURCE(IDB_BEARING_RIGHT_ONE_HD, 5510);
MAKE_RESOURCE(IDB_BEARING_RIGHT_TWO, 511);
MAKE_RESOURCE(IDB_BEARING_RIGHT_TWO_HD, 5511);
MAKE_RESOURCE(IDB_BEARING_RIGHT_THREE, 512);
MAKE_RESOURCE(IDB_BEARING_RIGHT_THREE_HD, 5512);
MAKE_RESOURCE(IDB_BEARING_RIGHT_FOUR, 513);
MAKE_RESOURCE(IDB_BEARING_RIGHT_FOUR_HD, 5513);
MAKE_RESOURCE(IDB_HAND_POINTER, 514);
MAKE_RESOURCE(IDB_HAND_POINTER_HD, 5514);
MAKE_RESOURCE(IDB_SCREENS_BUTTON, 515);
MAKE_RESOURCE(IDB_SCREENS_BUTTON_HD, 5515);


#ifdef USE_GDI
MAKE_RESOURCE(IDI_XCSOAR, 101);
MAKE_RESOURCE(IDB_ABOVETERRAIN, 321);
MAKE_RESOURCE(IDB_AIRSPACE, 235);
MAKE_RESOURCE(IDB_AIRSPACE0, 240);
MAKE_RESOURCE(IDB_AIRSPACE1, 241);
MAKE_RESOURCE(IDB_AIRSPACE2, 242);
MAKE_RESOURCE(IDB_AIRSPACE3, 280);
MAKE_RESOURCE(IDB_AIRSPACE4, 306);
MAKE_RESOURCE(IDB_AIRSPACE5, 311);
MAKE_RESOURCE(IDB_AIRSPACE6, 317);
MAKE_RESOURCE(IDB_AIRSPACE7, 318);
#endif

#undef MAKE_RESOURCE
