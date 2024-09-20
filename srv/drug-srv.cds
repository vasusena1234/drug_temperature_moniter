using drug.temperature as my from '../db/drug-model';

service drugSrv {
    @odata.draft.enabled  @odata.draft.bypass
    entity temperature as projection on my.Temperature_Moniter;
}
