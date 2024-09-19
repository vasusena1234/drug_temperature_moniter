namespace drug.temperature;


entity Temperature_Moniter {
    key sensorID    : Integer;
        temperature : Integer;
        createdAt   : Timestamp  @cds.on.insert: $now;
        modifiedAt  : Timestamp  @cds.on.insert: $now  @cds.on.update: $now;
}
