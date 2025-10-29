target_sources(taller_server
    PRIVATE
    # .cpp files
    vehicle_specs.yaml
    vehicle.cpp
    YamlParser.cpp
    PUBLIC
    vehicle.h
    constants.h
    vehicleDTO.h
    spawn.h
    YamlParser.h
    VehicleSpec.h
    # .h files
    )
