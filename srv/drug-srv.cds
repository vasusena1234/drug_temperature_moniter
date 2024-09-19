using drug.temperature as my from '../db/drug-model';

service drugSrv {
    entity temperature as projection on my.Temperature_Moniter;
}
