namespace drug.temperature;

using {cuid} from '@sap/cds/common';


entity Temperature_Moniter : cuid {
    sensorID       : Integer    @title: 'Sensor ID';
    temperatureInC : Decimal    @title: 'Temperate in celsius';
    temperatureInF : Decimal    @title: 'Temperate in Fahrenheit';
    humidity       : Decimal    @title: 'Humidity';
    createdAt      : Timestamp  @title: 'Created At Time'   @cds.on.insert: $now @readonly;
}
