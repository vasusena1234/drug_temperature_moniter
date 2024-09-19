namespace drug.temperature;


entity Temperature_Moniter {
    key sensorID    : Integer    @title: 'Sensor ID';
        temperature : Integer    @title: 'Temperate';
        createdAt   : Timestamp  @title: 'Created At Time'   @cds.on.insert: $now;
        modifiedAt  : Timestamp  @title: 'Modified At Time'  @cds.on.insert: $now  @cds.on.update: $now;
}
