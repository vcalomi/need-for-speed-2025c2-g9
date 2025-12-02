enable_testing()

add_executable(nfs_tests
  test_main.cpp
  test_protocols.cpp

  ../client/network/client_protocol.cpp
  ../server/network/server_protocol.cpp
  ../common/common_protocol.cpp
  ../common/socket.cpp
  ../common/resolver.cpp
  ../common/liberror.cpp
  ../common/resolvererror.cpp

  ../common/Dto/dto.cpp

  ../common/Dto/player_move.cpp
  ../common/Dto/initial_race_map.cpp
  ../common/Dto/player.cpp
  ../common/Dto/vehicle.cpp
  ../common/Dto/checkpoint.cpp
  ../common/Dto/vehicle_checkpoint.cpp
  ../common/Dto/vehicle_collision.cpp
  ../common/Dto/vehicle_wall_collision.cpp
  ../common/Dto/lap_completed.cpp
  ../common/Dto/race_finished.cpp
  ../common/Dto/vehicle_exploded.cpp
  ../common/Dto/player_race_finished.cpp
  ../common/Dto/infinite_health.cpp
  ../common/Dto/end_race.cpp
  ../common/Dto/vehicle_upgrade.cpp
  ../common/Dto/npc.cpp
  ../common/Dto/player_hit_npc.cpp
  ../common/Dto/player_game_finished.cpp

  ../common/serializer/serializer.cpp
  ../common/serializer/player_serializer.cpp
  ../common/serializer/player_move_serializer.cpp
  ../common/serializer/vehicle_serializer.cpp
  ../common/serializer/checkpoint_serializer.cpp
  ../common/serializer/vehicle_checkpoint_serializer.cpp
  ../common/serializer/vehicle_collision_serializer.cpp
  ../common/serializer/vehicle_wall_collision_serializer.cpp
  ../common/serializer/lap_completed_serializer.cpp
  ../common/serializer/race_finished_serializer.cpp
  ../common/serializer/vehicle_exploded_serializer.cpp
  ../common/serializer/initial_race_map_serializer.cpp
  ../common/serializer/player_race_finished_serializer.cpp
  ../common/serializer/player_left_serializer.cpp
  ../common/serializer/lobby_auth_serializer.cpp
  ../common/serializer/lobby_room_serializer.cpp
  ../common/serializer/lobby_room_state_serializer.cpp
  ../common/serializer/lobby_choose_car_serializer.cpp
  ../common/serializer/lobby_maps_serializer.cpp
  ../common/serializer/vehicle_upgrade_serializer.cpp
  ../common/serializer/infinite_health_serializer.cpp
  ../common/serializer/end_race_serializer.cpp
  ../common/serializer/npc_serializer.cpp
  ../common/serializer/player_hit_npc_serializer.cpp
  ../common/serializer/player_game_finished_serializer.cpp
)

target_link_libraries(nfs_tests PRIVATE
    nfs_common
    GTest::gtest
    GTest::gtest_main 
    pthread
)

include(GoogleTest)
gtest_discover_tests(nfs_tests)

add_test(NAME nfs_tests COMMAND nfs_tests)

add_custom_command(TARGET nfs_tests POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy
          $<TARGET_FILE:nfs_tests>
          ${CMAKE_BINARY_DIR}/nfs_tests
  COMMENT "Copying nfs_tests to ${CMAKE_BINARY_DIR}/nfs_tests")
