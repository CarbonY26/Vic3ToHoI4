#include <gmock/gmock-matchers.h>

#include "external/commonItems/external/googletest/googletest/include/gtest/gtest.h"
#include "src/hoi4_world/map/railways_converter.h"



namespace hoi4
{

TEST(Hoi4worldMapRailwaysConverterTests, IntrastateRailwaysAreCreated)
{
   const std::map<std::string, vic3::StateRegion> vic3_state_regions{
       {"STATE_ONE",
           vic3::StateRegion{{
                                 {"0x000001", "city"},
                                 {"0x000002", "dock"},
                                 {"0x000003", "mine"},
                             },
               {}}},
   };

   const mappers::ProvinceMapper province_mapper{{
                                                     {"0x000001", {1}},
                                                     {"0x000002", {2}},
                                                     {"0x000003", {3}},
                                                 },
       {}};

   const maps::MapData hoi4_map_data{{
       .province_neighbors =
           {
               {"1", {"2", "3"}},
           },
   }};

   const maps::ProvinceDefinitions hoi4_province_definitions{{
       .land_provinces = {"1", "2", "3"},
   }};

   const Railways railways =
       ConvertRailways(vic3_state_regions, province_mapper, hoi4_map_data, hoi4_province_definitions, {});

   EXPECT_THAT(railways.railways, testing::ElementsAre(Railway(3, {1, 2}), Railway(3, {1, 3})));
}


TEST(Hoi4worldMapRailwaysConverterTests, NoRailwaysThroughNonLandProvinces)
{
   const std::map<std::string, vic3::StateRegion> vic3_state_regions{
       {"STATE_ONE",
           vic3::StateRegion{{
                                 {"0x000001", "city"},
                                 {"0x000003", "dock"},
                             },
               {}}},
   };

   const mappers::ProvinceMapper province_mapper{{
                                                     {"0x000001", {1}},
                                                     {"0x000002", {2}},
                                                     {"0x000003", {3}},
                                                 },
       {}};

   const maps::MapData hoi4_map_data{{
       .province_neighbors =
           {
               {"1", {"2"}},
               {"2", {"1", "3"}},
               {"3", {"2"}},
           },
   }};

   const maps::ProvinceDefinitions hoi4_province_definitions{{
       .land_provinces = {"1", "3"},
   }};

   const Railways railways =
       ConvertRailways(vic3_state_regions, province_mapper, hoi4_map_data, hoi4_province_definitions, {});

   EXPECT_THAT(railways.railways, testing::ElementsAre());
}


TEST(Hoi4worldMapRailwaysConverterTests, NoCityMeansNoIntrastateConnections)
{
   const std::map<std::string, vic3::StateRegion> vic3_state_regions{
       {"STATE_ONE",
           vic3::StateRegion{{
                                 {"0x000001", "forest"},
                                 {"0x000002", "dock"},
                                 {"0x000003", "mine"},
                             },
               {}}},
   };

   const mappers::ProvinceMapper province_mapper{{
                                                     {"0x000001", {1}},
                                                     {"0x000002", {2}},
                                                     {"0x000003", {3}},
                                                 },
       {}};

   const maps::MapData hoi4_map_data{{
       .province_neighbors =
           {
               {"1", {"2", "3"}},
           },
   }};

   const maps::ProvinceDefinitions hoi4_province_definitions{{
       .land_provinces = {"1", "2", "3"},
   }};

   const Railways railways =
       ConvertRailways(vic3_state_regions, province_mapper, hoi4_map_data, hoi4_province_definitions, {});

   EXPECT_THAT(railways.railways, testing::ElementsAre());
}


TEST(Hoi4worldMapRailwaysConverterTests, NoIntrastateRailwaysToSameVic3Province)
{
   const std::map<std::string, vic3::StateRegion> vic3_state_regions{
       {"STATE_ONE",
           vic3::StateRegion{{
                                 {"0x000001", "city"},
                                 {"0x000001", "dock"},
                                 {"0x000001", "mine"},
                             },
               {}}},
   };

   const mappers::ProvinceMapper province_mapper{{
                                                     {"0x000001", {1}},
                                                     {"0x000002", {2}},
                                                     {"0x000003", {3}},
                                                 },
       {}};

   const maps::MapData hoi4_map_data{{
       .province_neighbors =
           {
               {"1", {"2", "3"}},
           },
   }};

   const maps::ProvinceDefinitions hoi4_province_definitions{{
       .land_provinces = {"1", "2", "3"},
   }};

   const Railways railways =
       ConvertRailways(vic3_state_regions, province_mapper, hoi4_map_data, hoi4_province_definitions, {});

   EXPECT_THAT(railways.railways, testing::ElementsAre());
}


TEST(Hoi4worldMapRailwaysConverterTests, NoIntrastateRailwaysToSameHoi4Province)
{
   const std::map<std::string, vic3::StateRegion> vic3_state_regions{
       {"STATE_ONE",
           vic3::StateRegion{{
                                 {"0x000001", "city"},
                                 {"0x000002", "dock"},
                                 {"0x000003", "mine"},
                             },
               {}}},
   };

   const mappers::ProvinceMapper province_mapper{{
                                                     {"0x000001", {1}},
                                                     {"0x000002", {1}},
                                                     {"0x000003", {1}},
                                                 },
       {}};

   const maps::MapData hoi4_map_data{{
       .province_neighbors =
           {
               {"1", {"2", "3"}},
           },
   }};

   const maps::ProvinceDefinitions hoi4_province_definitions{{
       .land_provinces = {"1", "2", "3"},
   }};

   const Railways railways =
       ConvertRailways(vic3_state_regions, province_mapper, hoi4_map_data, hoi4_province_definitions, {});

   EXPECT_THAT(railways.railways, testing::ElementsAre());
}


TEST(Hoi4worldMapRailwaysConverterTests, NoRailwayForUnmappedProvinces)
{
   const std::map<std::string, vic3::StateRegion> vic3_state_regions{
       {"STATE_ONE",
           vic3::StateRegion{{
                                 {"0x000001", "city"},
                                 {"0x000002", "dock"},
                                 {"0x000003", "mine"},
                             },
               {}}},
   };

   const mappers::ProvinceMapper province_mapper{{}, {}};

   const maps::MapData hoi4_map_data{{
       .province_neighbors =
           {
               {"1", {"2", "3"}},
           },
   }};

   const maps::ProvinceDefinitions hoi4_province_definitions{{
       .land_provinces = {"1", "2", "3"},
   }};

   std::stringstream log;
   std::streambuf* cout_buffer = std::cout.rdbuf();
   std::cout.rdbuf(log.rdbuf());

   const Railways railways =
       ConvertRailways(vic3_state_regions, province_mapper, hoi4_map_data, hoi4_province_definitions, {});

   std::cout.rdbuf(cout_buffer);

   EXPECT_THAT(railways.railways, testing::ElementsAre());
   EXPECT_THAT(log.str(), testing::HasSubstr("No hoi4 provinces for significant vic3 province 0x000001 or 0x000002."));
   EXPECT_THAT(log.str(), testing::HasSubstr("No hoi4 provinces for significant vic3 province 0x000001 or 0x000003."));
}


TEST(Hoi4worldMapRailwaysConverterTests, NoRailwayForProvincesMappedToNothing)
{
   const std::map<std::string, vic3::StateRegion> vic3_state_regions{
       {"STATE_ONE",
           vic3::StateRegion{{
                                 {"0x000001", "city"},
                                 {"0x000002", "dock"},
                                 {"0x000003", "mine"},
                             },
               {}}},
   };

   const mappers::ProvinceMapper province_mapper{{
                                                     {"0x000001", {}},
                                                     {"0x000002", {}},
                                                     {"0x000003", {}},
                                                 },
       {}};

   const maps::MapData hoi4_map_data{{
       .province_neighbors =
           {
               {"1", {"2", "3"}},
           },
   }};

   const maps::ProvinceDefinitions hoi4_province_definitions{{
       .land_provinces = {"1", "2", "3"},
   }};

   const Railways railways =
       ConvertRailways(vic3_state_regions, province_mapper, hoi4_map_data, hoi4_province_definitions, {});

   EXPECT_THAT(railways.railways, testing::ElementsAre());
}


TEST(Hoi4worldMapRailwaysConverterTests, NoIntrastateRailwaysForDisconnectedProvinces)
{
   const std::map<std::string, vic3::StateRegion> vic3_state_regions{
       {"STATE_ONE",
           vic3::StateRegion{{
                                 {"0x000001", "city"},
                                 {"0x000002", "dock"},
                                 {"0x000003", "mine"},
                             },
               {}}},
   };

   const mappers::ProvinceMapper province_mapper{{
                                                     {"0x000001", {1}},
                                                     {"0x000002", {2}},
                                                     {"0x000003", {3}},
                                                 },
       {}};

   const maps::MapData hoi4_map_data;

   const maps::ProvinceDefinitions hoi4_province_definitions{{
       .land_provinces = {"1", "2", "3"},
   }};

   const Railways railways =
       ConvertRailways(vic3_state_regions, province_mapper, hoi4_map_data, hoi4_province_definitions, {});

   EXPECT_THAT(railways.railways, testing::ElementsAre());
}


TEST(Hoi4worldMapRailwaysConverterTests, InterstateRailwaysAreCreated)
{
   const std::map<std::string, vic3::StateRegion> vic3_state_regions{
       {"STATE_ONE",
           vic3::StateRegion{{
                                 {"0x000001", "city"},
                             },
               {}}},
       {"STATE_TWO",
           vic3::StateRegion{{
                                 {"0x000010", "city"},
                             },
               {}}},
       {"STATE_THREE",
           vic3::StateRegion{{
                                 {"0x000100", "city"},
                             },
               {}}},
   };

   const mappers::ProvinceMapper province_mapper{{
                                                     {"0x000001", {1}},
                                                     {"0x000010", {10}},
                                                     {"0x000100", {100}},
                                                 },
       {}};

   const maps::MapData hoi4_map_data{{
       .province_neighbors =
           {
               {"1", {"10", "100", "bad_neighbor_to_verify_catch"}},
               {"10", {"1"}},
               {"100", {"1"}},
           },
   }};

   const maps::ProvinceDefinitions hoi4_province_definitions{{
       .land_provinces = {"1", "10", "100"},
   }};

   const hoi4::States hoi4_states{
       .states =
           {
               hoi4::State(1, {}),
               hoi4::State(2, {}),
               hoi4::State(3, {}),
           },
       .province_to_state_id_map =
           {
               {1, 1},
               {10, 2},
               {100, 3},
           },
   };

   const Railways railways =
       ConvertRailways(vic3_state_regions, province_mapper, hoi4_map_data, hoi4_province_definitions, hoi4_states);

   EXPECT_THAT(railways.railways, testing::ElementsAre(Railway(3, {1, 10}), Railway(3, {1, 100})));
}


TEST(Hoi4worldMapRailwaysConverterTests, NoInterstateRailroadsForNonBorderingStates)
{
   const std::map<std::string, vic3::StateRegion> vic3_state_regions{
       {"STATE_ONE",
           vic3::StateRegion{{
                                 {"0x000001", "city"},
                             },
               {}}},
       {"STATE_TWO",
           vic3::StateRegion{{
                                 {"0x000010", "city"},
                             },
               {}}},
       {"STATE_THREE",
           vic3::StateRegion{{
                                 {"0x000100", "city"},
                             },
               {}}},
   };

   const mappers::ProvinceMapper province_mapper{{
                                                     {"0x000001", {1}},
                                                     {"0x000010", {10}},
                                                     {"0x000100", {100}},
                                                 },
       {}};

   const maps::MapData hoi4_map_data;

   const maps::ProvinceDefinitions hoi4_province_definitions{{
       .land_provinces = {"1", "10", "100"},
   }};

   const hoi4::States hoi4_states{
       .states = {hoi4::State(1, {}), hoi4::State(2, {})},
       .province_to_state_id_map =
           {
               {1, 1},
               {10, 2},
               {100, 3},
           },
   };

   const Railways railways =
       ConvertRailways(vic3_state_regions, province_mapper, hoi4_map_data, hoi4_province_definitions, hoi4_states);

   EXPECT_THAT(railways.railways, testing::ElementsAre());
}


TEST(Hoi4worldMapRailwaysConverterTests, NoInterstateRailroadsForStatesWithoutSignificantProvinces)
{
   const std::map<std::string, vic3::StateRegion> vic3_state_regions{
       {"STATE_ONE", vic3::StateRegion{{}, {}}},
       {"STATE_TWO",
           vic3::StateRegion{{
                                 {"0x000010", "city"},
                             },
               {}}},
       {"STATE_THREE",
           vic3::StateRegion{{
                                 {"0x000100", "city"},
                             },
               {}}},
   };

   const mappers::ProvinceMapper province_mapper{{
                                                     {"0x000001", {1}},
                                                     {"0x000010", {10}},
                                                     {"0x000100", {100}},
                                                 },
       {}};

   const maps::MapData hoi4_map_data{{
       .province_neighbors =
           {
               {"1", {"10", "100"}},
               {"10", {"1"}},
               {"100", {"1"}},
           },
   }};

   const maps::ProvinceDefinitions hoi4_province_definitions{{
       .land_provinces = {"1", "10", "100"},
   }};

   const hoi4::States hoi4_states{
       .states = {hoi4::State(1, {}), hoi4::State(2, {})},
       .province_to_state_id_map =
           {
               {1, 1},
               {10, 2},
               {100, 3},
           },
   };

   const Railways railways =
       ConvertRailways(vic3_state_regions, province_mapper, hoi4_map_data, hoi4_province_definitions, hoi4_states);

   EXPECT_THAT(railways.railways, testing::ElementsAre());
}


TEST(Hoi4worldMapRailwaysConverterTests, NoInterstateRailwaysForRepeatedSignificantProvince)
{
   const std::map<std::string, vic3::StateRegion> vic3_state_regions{
       {"STATE_ONE",
           vic3::StateRegion{{
                                 {"0x000001", "city"},
                             },
               {}}},
       {"STATE_TWO",
           vic3::StateRegion{{
                                 {"0x000010", "city"},
                             },
               {}}},
       {"STATE_THREE",
           vic3::StateRegion{{
                                 {"0x000100", "city"},
                             },
               {}}},
   };

   const mappers::ProvinceMapper province_mapper{{
                                                     {"0x000001", {1}},
                                                     {"0x000010", {1}},
                                                     {"0x000100", {1}},
                                                 },
       {}};

   const maps::MapData hoi4_map_data{{
       .province_neighbors = {},
   }};

   const maps::ProvinceDefinitions hoi4_province_definitions{{
       .land_provinces = {"1"},
   }};

   const hoi4::States hoi4_states{
       .states = {hoi4::State(1, {}), hoi4::State(2, {})},
       .province_to_state_id_map =
           {
               {1, 1},
           },
   };

   const Railways railways =
       ConvertRailways(vic3_state_regions, province_mapper, hoi4_map_data, hoi4_province_definitions, hoi4_states);

   EXPECT_THAT(railways.railways, testing::ElementsAre());
}


TEST(Hoi4worldMapRailwaysConverterTests, InterstateRailroadsAreDeduplicated)
{
   const std::map<std::string, vic3::StateRegion> vic3_state_regions{
       {"STATE_ONE",
           vic3::StateRegion{{
                                 {"0x000001", "city"},
                             },
               {}}},
       {"STATE_TWO",
           vic3::StateRegion{{
                                 {"0x000010", "city"},
                             },
               {}}},
       {"STATE_THREE",
           vic3::StateRegion{{
                                 {"0x000100", "city"},
                             },
               {}}},
   };

   const mappers::ProvinceMapper province_mapper{{
                                                     {"0x000001", {1}},
                                                     {"0x000010", {10}},
                                                     {"0x000100", {10}},
                                                 },
       {}};

   const maps::MapData hoi4_map_data{{
       .province_neighbors =
           {
               {"1", {"10"}},
               {"10", {"1"}},
           },
   }};

   const maps::ProvinceDefinitions hoi4_province_definitions{{
       .land_provinces = {"1", "10"},
   }};

   const hoi4::States hoi4_states{
       .states =
           {
               hoi4::State(1, {}),
               hoi4::State(2, {}),
               hoi4::State(3, {}),
           },
       .province_to_state_id_map =
           {
               {1, 1},
               {10, 2},
           },
   };

   const Railways railways =
       ConvertRailways(vic3_state_regions, province_mapper, hoi4_map_data, hoi4_province_definitions, hoi4_states);

   EXPECT_THAT(railways.railways, testing::ElementsAre(Railway(3, {1, 10})));
}


TEST(Hoi4worldMapRailwaysConverterTests, LowestCostConnectionFormsInterstateRailroad)
{
   const std::map<std::string, vic3::StateRegion> vic3_state_regions{
       {"STATE_ONE",
           vic3::StateRegion{{
                                 {"0x000001", "city"},
                                 {"0x000002", "port"},
                                 {"0x000003", "mine"},
                             },
               {}}},
       {"STATE_TWO",
           vic3::StateRegion{{
                                 {"0x000010", "city"},
                                 {"0x000020", "port"},
                                 {"0x000030", "mine"},
                             },
               {}}},
   };

   const mappers::ProvinceMapper province_mapper{{
                                                     {"0x000001", {1}},
                                                     {"0x000002", {2}},
                                                     {"0x000003", {3}},
                                                     {"0x000010", {10}},
                                                     {"0x000020", {20}},
                                                     {"0x000030", {30}},
                                                 },
       {}};

   const maps::MapData hoi4_map_data{{
       .province_neighbors =
           {
               {"1", {"100"}},
               {"100", {"1000", "1"}},
               {"1000", {"100", "10"}},
               {"10", {"1000"}},
               {"2", {"20"}},
               {"20", {"2"}},
               {"3", {"300"}},
               {"300", {"30", "3"}},
               {"30", {"300"}},
           },
   }};

   const maps::ProvinceDefinitions hoi4_province_definitions{{
       .land_provinces = {"1", "10", "100", "1000", "2", "20", "3", "30", "300"},
   }};

   const hoi4::States hoi4_states{
       .states = {hoi4::State(1, {}), hoi4::State(2, {})},
       .province_to_state_id_map =
           {
               {1, 1},
               {100, 1},
               {2, 1},
               {3, 1},
               {300, 1},
               {10, 2},
               {1000, 2},
               {20, 2},
               {30, 2},
           },
   };

   const Railways railways =
       ConvertRailways(vic3_state_regions, province_mapper, hoi4_map_data, hoi4_province_definitions, hoi4_states);

   EXPECT_THAT(railways.railways, testing::ElementsAre(Railway(3, {2, 20})));
}


TEST(Hoi4worldMapRailwaysConverterTests, RailwaysPreferShorterPaths)
{
   const std::map<std::string, vic3::StateRegion> vic3_state_regions{
       {"STATE_ONE",
           vic3::StateRegion{{
                                 {"0x000001", "city"},
                                 {"0x000010", "dock"},
                             },
               {}}},
   };

   const mappers::ProvinceMapper province_mapper{{
                                                     {"0x000001", {1}},
                                                     {"0x000010", {10}},
                                                 },
       {}};

   const maps::MapData hoi4_map_data{{
       .province_neighbors =
           {
               {"1", {"2", "3"}},
               {"2", {"1", "10"}},
               {"3", {"1", "10"}},
               {"10", {"2", "3"}},
           },
       .the_province_points =
           {
               {"1", maps::ProvincePoints(std::set<maps::Point>{{.x = 0, .y = 0}})},
               {"2", maps::ProvincePoints(std::set<maps::Point>{{.x = 0, .y = 1}})},
               {"3", maps::ProvincePoints(std::set<maps::Point>{{.x = 10, .y = 1}})},
               {"10", maps::ProvincePoints(std::set<maps::Point>{{.x = 0, .y = 2}})},
           },
   }};

   const maps::ProvinceDefinitions hoi4_province_definitions{{
       .land_provinces = {"1", "2", "3", "10"},
   }};

   const Railways railways =
       ConvertRailways(vic3_state_regions, province_mapper, hoi4_map_data, hoi4_province_definitions, {});

   EXPECT_THAT(railways.railways, testing::ElementsAre(Railway(3, {1, 2, 10})));
}


TEST(Hoi4worldMapRailwaysConverterTests, RailwaysPreferFewerProvincesIfDistanceIndeterminate)
{
   const std::map<std::string, vic3::StateRegion> vic3_state_regions{
       {"STATE_ONE",
           vic3::StateRegion{{
                                 {"0x000001", "city"},
                                 {"0x000010", "dock"},
                             },
               {}}},
   };

   const mappers::ProvinceMapper province_mapper{{
                                                     {"0x000001", {1}},
                                                     {"0x000010", {10}},
                                                 },
       {}};

   const maps::MapData hoi4_map_data{{
       .province_neighbors =
           {
               {"1", {"2", "6"}},
               {"2", {"1", "3"}},
               {"3", {"2", "4"}},
               {"4", {"3", "10"}},
               {"6", {"1", "7"}},
               {"7", {"6", "10"}},
               {"10", {"4", "7"}},
           },
   }};

   const maps::ProvinceDefinitions hoi4_province_definitions{{
       .land_provinces = {"1", "2", "3", "4", "5", "6", "7", "10"},
   }};

   const Railways railways =
       ConvertRailways(vic3_state_regions, province_mapper, hoi4_map_data, hoi4_province_definitions, {});

   EXPECT_THAT(railways.railways, testing::ElementsAre(Railway(3, {1, 6, 7, 10})));
}


TEST(Hoi4worldMapRailwaysConverterTests, RailwaysPreferEasierTerrains)
{
   const std::map<std::string, vic3::StateRegion> vic3_state_regions{
       {"STATE_ONE",
           vic3::StateRegion{{
                                 {"0x100000", "city"},
                                 {"0x000100", "urban_vs_plains"},
                                 {"0x000200", "plains_vs_forest"},
                                 {"0x000300", "forest_vs_hills"},
                                 {"0x000400", "hills_vs_desert"},
                                 {"0x000500", "desert_vs_marsh"},
                                 {"0x000600", "marsh_vs_jungle"},
                                 {"0x000700", "jungle_ms_mountain"},
                                 {"0x000800", "mountain_vs_unhandled"},
                             },
               {}}},
   };

   const mappers::ProvinceMapper province_mapper{{
                                                     {"0x100000", {100000}},
                                                     {"0x000100", {100}},
                                                     {"0x000200", {200}},
                                                     {"0x000300", {300}},
                                                     {"0x000400", {400}},
                                                     {"0x000500", {500}},
                                                     {"0x000600", {600}},
                                                     {"0x000700", {700}},
                                                     {"0x000800", {800}},
                                                 },
       {}};
   const std::map<std::string, std::set<std::string>> province_neighbors = {
       {"100000", {"1", "10", "2", "20", "3", "30", "4", "40", "5", "50", "6", "60", "7", "70", "8", "80"}},
       {"1", {"100000", "100"}},
       {"2", {"100000", "200"}},
       {"3", {"100000", "300"}},
       {"4", {"100000", "400"}},
       {"5", {"100000", "500"}},
       {"6", {"100000", "600"}},
       {"7", {"100000", "700"}},
       {"8", {"100000", "800"}},
       {"10", {"100000", "100"}},
       {"20", {"100000", "200"}},
       {"30", {"100000", "300"}},
       {"40", {"100000", "400"}},
       {"50", {"100000", "500"}},
       {"60", {"100000", "600"}},
       {"70", {"100000", "700"}},
       {"80", {"100000", "800"}},
       {"100", {"1", "10"}},
       {"200", {"2", "20"}},
       {"300", {"3", "30"}},
       {"400", {"4", "40"}},
       {"500", {"5", "50"}},
       {"600", {"6", "60"}},
       {"700", {"7", "70"}},
       {"800", {"8", "80"}},
   };
   const std::unordered_map<std::string, maps::ProvincePoints> the_province_points = {
       {"100000", maps::ProvincePoints(std::set<maps::Point>{{.x = 1, .y = 0}})},
       {"2", maps::ProvincePoints(std::set<maps::Point>{{.x = 0, .y = 1}})},
       {"20", maps::ProvincePoints(std::set<maps::Point>{{.x = 2, .y = 1}})},
       {"3", maps::ProvincePoints(std::set<maps::Point>{{.x = 0, .y = 1}})},
       {"30", maps::ProvincePoints(std::set<maps::Point>{{.x = 2, .y = 1}})},
       {"4", maps::ProvincePoints(std::set<maps::Point>{{.x = 0, .y = 1}})},
       {"40", maps::ProvincePoints(std::set<maps::Point>{{.x = 2, .y = 1}})},
       {"5", maps::ProvincePoints(std::set<maps::Point>{{.x = 0, .y = 1}})},
       {"50", maps::ProvincePoints(std::set<maps::Point>{{.x = 2, .y = 1}})},
       {"6", maps::ProvincePoints(std::set<maps::Point>{{.x = 0, .y = 1}})},
       {"60", maps::ProvincePoints(std::set<maps::Point>{{.x = 2, .y = 1}})},
       {"7", maps::ProvincePoints(std::set<maps::Point>{{.x = 0, .y = 1}})},
       {"70", maps::ProvincePoints(std::set<maps::Point>{{.x = 2, .y = 1}})},
       {"8", maps::ProvincePoints(std::set<maps::Point>{{.x = 0, .y = 1}})},
       {"80", maps::ProvincePoints(std::set<maps::Point>{{.x = 2, .y = 1}})},
       {"100", maps::ProvincePoints(std::set<maps::Point>{{.x = 1, .y = 2}})},
       {"200", maps::ProvincePoints(std::set<maps::Point>{{.x = 1, .y = 2}})},
       {"300", maps::ProvincePoints(std::set<maps::Point>{{.x = 1, .y = 2}})},
       {"400", maps::ProvincePoints(std::set<maps::Point>{{.x = 1, .y = 2}})},
       {"500", maps::ProvincePoints(std::set<maps::Point>{{.x = 1, .y = 2}})},
       {"600", maps::ProvincePoints(std::set<maps::Point>{{.x = 1, .y = 2}})},
       {"700", maps::ProvincePoints(std::set<maps::Point>{{.x = 1, .y = 2}})},
       {"800", maps::ProvincePoints(std::set<maps::Point>{{.x = 1, .y = 2}})},
   };
   const std::set<std::string> land_provinces = {
       "100000",
       "1",
       "2",
       "3",
       "4",
       "5",
       "6",
       "7",
       "8",
       "10",
       "20",
       "30",
       "40",
       "50",
       "60",
       "70",
       "80",
       "100",
       "200",
       "300",
       "400",
       "500",
       "600",
       "700",
       "800",
   };
   const maps::ProvinceDefinitions province_definitions({
       .land_provinces = land_provinces,
       .terrain_types =
           {
               {"1", "plains"},
               {"10", "urban"},
               {"2", "forest"},
               {"20", "plains"},
               {"3", "hills"},
               {"30", "forest"},
               {"4", "desert"},
               {"40", "hills"},
               {"5", "marsh"},
               {"50", "desert"},
               {"6", "jungle"},
               {"60", "marsh"},
               {"7", "mountain"},
               {"70", "jungle"},
               {"8", "unhandled"},
               {"80", "mountain"},
           },
   });
   const maps::MapData hoi4_map_data{{
       .province_neighbors = province_neighbors,
       .the_province_points = the_province_points,
       .province_definitions = province_definitions,
   }};

   const Railways railways =
       ConvertRailways(vic3_state_regions, province_mapper, hoi4_map_data, province_definitions, {});

   EXPECT_THAT(railways.railways,
       testing::ElementsAre(Railway(3, {100000, 10, 100}),
           Railway(3, {100000, 20, 200}),
           Railway(3, {100000, 30, 300}),
           Railway(3, {100000, 40, 400}),
           Railway(3, {100000, 50, 500}),
           Railway(3, {100000, 60, 600}),
           Railway(3, {100000, 70, 700}),
           Railway(3, {100000, 80, 800})));
}


TEST(Hoi4worldMapRailwaysConverterTests, RailwayEndpointsAreRecoded)
{
   const std::map<std::string, vic3::StateRegion> vic3_state_regions{
       {"STATE_ONE",
           vic3::StateRegion{{
                                 {"0x000001", "city"},
                                 {"0x000002", "dock"},
                                 {"0x000003", "mine"},
                             },
               {}}},
   };

   const mappers::ProvinceMapper province_mapper{{
                                                     {"0x000001", {1}},
                                                     {"0x000002", {2}},
                                                     {"0x000003", {3}},
                                                 },
       {}};

   const maps::MapData hoi4_map_data{{
       .province_neighbors =
           {
               {"1", {"2", "3"}},
           },
   }};

   const maps::ProvinceDefinitions hoi4_province_definitions{{
       .land_provinces = {"1", "2", "3"},
   }};

   const Railways railways =
       ConvertRailways(vic3_state_regions, province_mapper, hoi4_map_data, hoi4_province_definitions, {});

   EXPECT_THAT(railways.railways, testing::ElementsAre(Railway(3, {1, 2}), Railway(3, {1, 3})));
   EXPECT_THAT(railways.railway_endpoints, testing::ElementsAre(1, 2, 3));
}

}  // namespace hoi4