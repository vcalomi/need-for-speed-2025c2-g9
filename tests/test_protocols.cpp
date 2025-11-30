#include <gtest/gtest.h>
#include <memory>
#include <thread>

#include "../server/network/server_protocol.h"
#include "../client/network/client_protocol.h"
#include "../common/socket.h"
#include "../common/Dto/player_move.h"
#include "../common/Dto/player_left.h"
#include "../common/Dto/initial_race_map.h"
#include "../common/Dto/lobby_auth.h"
#include "../common/Dto/lobby_room.h"
#include "../common/Dto/lobby_room_state.h"
#include "../common/Dto/lap_completed.h"
#include "../common/Dto/race_finished.h"
#include "../common/Dto/vehicle_checkpoint.h"
#include "../common/Dto/vehicle.h"
#include "../common/Dto/vehicle_collision.h"
#include "../common/Dto/vehicle_wall_collision.h"
#include "../common/Dto/vehicle_exploded.h"
#include "../common/Dto/checkpoint.h"

const char* SERVER_TEST_PORT = "18083";
const char* IP = "localhost";

class ProtocolTest : public ::testing::Test {
protected:
    void SetUp() override {
        listener = std::make_unique<Socket>(SERVER_TEST_PORT);
        client_protocol = std::make_unique<ClientProtocol>(IP, SERVER_TEST_PORT);
        accepted_socket = std::make_unique<Socket>(listener->accept());
        server_protocol = std::make_unique<ServerProtocol>(*accepted_socket);
    }

    void TearDown() override {
        if (client_protocol) {
            client_protocol->close();
        }
        server_protocol.reset();
        accepted_socket.reset();
        client_protocol.reset();
        listener.reset();
    }

    std::unique_ptr<Socket> listener;
    std::unique_ptr<Socket> accepted_socket;
    std::unique_ptr<ServerProtocol> server_protocol;
    std::unique_ptr<ClientProtocol> client_protocol;
};


TEST_F(ProtocolTest, CREATE_GAME_AND_START_GAME) {
    auto authDto = std::make_shared<AuthDto>(static_cast<uint8_t>(ActionCode::SEND_USERNAME));
    authDto->username = "test_user";
    ASSERT_NO_THROW(client_protocol->sendDTO(authDto));

    auto authReq = server_protocol->receiveDTO();
    ASSERT_NE(authReq, nullptr);
    ASSERT_EQ(static_cast<ActionCode>(authReq->return_code()), ActionCode::SEND_USERNAME);

    auto authOk = std::make_shared<AuthDto>(static_cast<uint8_t>(ActionCode::USERNAME_OK));
    ASSERT_NO_THROW(server_protocol->sendDTO(authOk));

    auto ok1 = client_protocol->receiveDTO();
    ASSERT_NE(ok1, nullptr);
    ASSERT_EQ(static_cast<ActionCode>(ok1->return_code()), ActionCode::USERNAME_OK);

    auto createDto = std::make_shared<RoomDto>(static_cast<uint8_t>(ActionCode::CREATE_ROOM));
    createDto->roomCode = "TestRoom";
    ASSERT_NO_THROW(client_protocol->sendDTO(createDto));

    auto createReq = server_protocol->receiveDTO();
    ASSERT_NE(createReq, nullptr);
    ASSERT_EQ(static_cast<ActionCode>(createReq->return_code()), ActionCode::CREATE_ROOM);

    auto created = std::make_shared<RoomDto>(static_cast<uint8_t>(ActionCode::ROOM_CREATED));
    created->roomCode = "TestRoom";
    ASSERT_NO_THROW(server_protocol->sendDTO(created));

    auto ok2 = client_protocol->receiveDTO();
    ASSERT_NE(ok2, nullptr);
    ASSERT_EQ(static_cast<ActionCode>(ok2->return_code()), ActionCode::ROOM_CREATED);

    auto startDto = std::make_shared<RoomDto>(static_cast<uint8_t>(ActionCode::START_GAME));
    ASSERT_NO_THROW(client_protocol->sendDTO(startDto));

    auto startReq = server_protocol->receiveDTO();
    ASSERT_NE(startReq, nullptr);
    ASSERT_EQ(static_cast<ActionCode>(startReq->return_code()), ActionCode::START_GAME);
}

TEST_F(ProtocolTest, LIST_GAMES_AND_JOIN_GAME) {
    auto authDto = std::make_shared<AuthDto>(static_cast<uint8_t>(ActionCode::SEND_USERNAME));
    authDto->username = "test_user2";
    ASSERT_NO_THROW(client_protocol->sendDTO(authDto));

    auto authReq = server_protocol->receiveDTO();
    ASSERT_NE(authReq, nullptr);
    
    auto authOk = std::make_shared<AuthDto>(static_cast<uint8_t>(ActionCode::USERNAME_OK));
    ASSERT_NO_THROW(server_protocol->sendDTO(authOk));
    
    auto ok1 = client_protocol->receiveDTO();
    ASSERT_NE(ok1, nullptr);

    auto listDto = std::make_shared<RoomDto>(static_cast<uint8_t>(ActionCode::LIST_ROOMS));
    ASSERT_NO_THROW(client_protocol->sendDTO(listDto));

    auto listReq = server_protocol->receiveDTO();
    ASSERT_NE(listReq, nullptr);
    ASSERT_EQ(static_cast<ActionCode>(listReq->return_code()), ActionCode::LIST_ROOMS);

    auto roomsDto = std::make_shared<RoomDto>(static_cast<uint8_t>(ActionCode::LIST_ROOMS));
    roomsDto->rooms = {"R1", "R2"};
    ASSERT_NO_THROW(server_protocol->sendDTO(roomsDto));

    auto gotRooms = client_protocol->receiveDTO();
    ASSERT_NE(gotRooms, nullptr);
    ASSERT_EQ(static_cast<ActionCode>(gotRooms->return_code()), ActionCode::LIST_ROOMS);

    auto joinDto = std::make_shared<RoomDto>(static_cast<uint8_t>(ActionCode::JOIN_ROOM));
    joinDto->roomCode = "R1";
    ASSERT_NO_THROW(client_protocol->sendDTO(joinDto));

    auto joinReq = server_protocol->receiveDTO();
    ASSERT_NE(joinReq, nullptr);
    ASSERT_EQ(static_cast<ActionCode>(joinReq->return_code()), ActionCode::JOIN_ROOM);

    auto joinOk = std::make_shared<RoomDto>(static_cast<uint8_t>(ActionCode::JOIN_OK));
    ASSERT_NO_THROW(server_protocol->sendDTO(joinOk));

    auto ok = client_protocol->receiveDTO();
    ASSERT_NE(ok, nullptr);
    ASSERT_EQ(static_cast<ActionCode>(ok->return_code()), ActionCode::JOIN_OK);
}

TEST_F(ProtocolTest, SEND_INITIAL_SNAPSHOT) {
    auto authDto = std::make_shared<AuthDto>(static_cast<uint8_t>(ActionCode::SEND_USERNAME));
    authDto->username = "test_user";
    client_protocol->sendDTO(authDto);
    
    auto authReq = server_protocol->receiveDTO();
    auto authOk = std::make_shared<AuthDto>(static_cast<uint8_t>(ActionCode::USERNAME_OK));
    server_protocol->sendDTO(authOk);
    client_protocol->receiveDTO();

    auto out = std::make_shared<InitialRaceMapDto>(std::string("liberty_city"));
    ASSERT_NO_THROW(server_protocol->sendDTO(out));
    
    auto in = client_protocol->receiveDTO();
    ASSERT_NE(in, nullptr);
    EXPECT_EQ(static_cast<ActionCode>(in->return_code()), ActionCode::SEND_INITIAL_RACE_MAP);
    
    auto dto = std::dynamic_pointer_cast<InitialRaceMapDto>(in);
    ASSERT_NE(dto, nullptr);
    EXPECT_EQ(dto->mapName, "liberty_city");
}

TEST_F(ProtocolTest, RECEIVE_PLAYER_MOVE) {
    auto authDto = std::make_shared<AuthDto>(static_cast<uint8_t>(ActionCode::SEND_USERNAME));
    authDto->username = "test_user";
    client_protocol->sendDTO(authDto);
    
    auto authReq = server_protocol->receiveDTO();
    auto authOk = std::make_shared<AuthDto>(static_cast<uint8_t>(ActionCode::USERNAME_OK));
    server_protocol->sendDTO(authOk);
    client_protocol->receiveDTO();

    auto out = std::make_shared<PlayerMoveDto>(std::string("juana"), 0);
    ASSERT_NO_THROW(client_protocol->sendDTO(out));
    
    auto in = server_protocol->receiveDTO();
    ASSERT_NE(in, nullptr);
    EXPECT_EQ(static_cast<ActionCode>(in->return_code()), ActionCode::SEND_PLAYER_MOVE);
    
    auto dto = std::dynamic_pointer_cast<PlayerMoveDto>(in);
    ASSERT_NE(dto, nullptr);
    EXPECT_EQ(dto->username, "juana");
    EXPECT_EQ(dto->move, 0);
}

TEST_F(ProtocolTest, GAMEPLAY_VEHICLE_DTO) {
    auto vehicleDto = std::make_shared<VehicleDto>("player1", 100.5f, 200.3f, 45.0f, 150.0f, false);
    ASSERT_NO_THROW(server_protocol->sendDTO(vehicleDto));
    
    auto receivedVehicle = client_protocol->receiveDTO();
    ASSERT_NE(receivedVehicle, nullptr);
    EXPECT_EQ(static_cast<ActionCode>(receivedVehicle->return_code()), ActionCode::SEND_CARS);
    
    auto vehicle = std::dynamic_pointer_cast<VehicleDto>(receivedVehicle);
    ASSERT_NE(vehicle, nullptr);
    EXPECT_EQ(vehicle->username, "player1");
    EXPECT_FLOAT_EQ(vehicle->x, 100.5f);
    EXPECT_FLOAT_EQ(vehicle->y, 200.3f);
    EXPECT_FLOAT_EQ(vehicle->rotation, 45.0f);
    EXPECT_TRUE(vehicle->isAboveBridge);
}

TEST_F(ProtocolTest, GAMEPLAY_RACE_EVENTS) {
    auto checkpointDto = std::make_shared<CheckpointDto>(2, 150.0f, 300.0f);
    ASSERT_NO_THROW(server_protocol->sendDTO(checkpointDto));
    
    auto receivedCheckpoint = client_protocol->receiveDTO();
    ASSERT_NE(receivedCheckpoint, nullptr);
    EXPECT_EQ(static_cast<ActionCode>(receivedCheckpoint->return_code()), ActionCode::SEND_CHECKPOINTS);
    
    auto checkpoint = std::dynamic_pointer_cast<CheckpointDto>(receivedCheckpoint);
    ASSERT_NE(checkpoint, nullptr);
    EXPECT_EQ(checkpoint->id, 2);
    EXPECT_FLOAT_EQ(checkpoint->x, 150.0f);
    EXPECT_FLOAT_EQ(checkpoint->y, 300.0f);
    
    auto lapDto = std::make_shared<LapCompletedDto>("player1", 2);
    ASSERT_NO_THROW(server_protocol->sendDTO(lapDto));
    
    auto receivedLap = client_protocol->receiveDTO();
    ASSERT_NE(receivedLap, nullptr);
    EXPECT_EQ(static_cast<ActionCode>(receivedLap->return_code()), ActionCode::SEND_LAP_COMPLETED);
    
    auto lap = std::dynamic_pointer_cast<LapCompletedDto>(receivedLap);
    ASSERT_NE(lap, nullptr);
    EXPECT_EQ(lap->username, "player1");
    EXPECT_EQ(lap->current_lap, 2);
}

TEST_F(ProtocolTest, GAMEPLAY_COLLISION_EVENTS) {
    auto collisionDto = std::make_shared<VehicleCollisionDto>("player1", 75.5f, "player2", 50.0f);
    ASSERT_NO_THROW(server_protocol->sendDTO(collisionDto));
    
    auto receivedCollision = client_protocol->receiveDTO();
    ASSERT_NE(receivedCollision, nullptr);
    EXPECT_EQ(static_cast<ActionCode>(receivedCollision->return_code()), ActionCode::SEND_VEHICLES_COLLISION);
    
    auto collision = std::dynamic_pointer_cast<VehicleCollisionDto>(receivedCollision);
    ASSERT_NE(collision, nullptr);
    EXPECT_EQ(collision->vehicle1_username, "player1");
    EXPECT_FLOAT_EQ(collision->vehicle_1_new_hp, 75.5f);
    EXPECT_EQ(collision->vehicle2_username, "player2");
    EXPECT_FLOAT_EQ(collision->vehicle_2_new_hp, 50.0f);

    auto explodedDto = std::make_shared<VehicleExplodedDto>("player1");
    ASSERT_NO_THROW(server_protocol->sendDTO(explodedDto));
    
    auto receivedExploded = client_protocol->receiveDTO();
    ASSERT_NE(receivedExploded, nullptr);
    EXPECT_EQ(static_cast<ActionCode>(receivedExploded->return_code()), ActionCode::SEND_VEHICLE_EXPLODED);
    
    auto exploded = std::dynamic_pointer_cast<VehicleExplodedDto>(receivedExploded);
    ASSERT_NE(exploded, nullptr);
    EXPECT_EQ(exploded->username, "player1");
}