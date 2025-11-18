enable_testing()

add_executable(runTests
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
)

find_package(GTest REQUIRED)
target_link_libraries(runTests PRIVATE GTest::gtest GTest::gtest_main pthread)

add_test(NAME runTests COMMAND runTests)

add_custom_command(TARGET runTests POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy
          $<TARGET_FILE:runTests>
          ${CMAKE_BINARY_DIR}/taller_tests
  COMMENT "Copying runTests to ${CMAKE_BINARY_DIR}/taller_tests"
)