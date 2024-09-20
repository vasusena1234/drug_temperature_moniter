using drugSrv as service from '../../srv/drug-srv';

annotate service.temperature with @(
    UI.FieldGroup #GeneratedGroup: {
        $Type: 'UI.FieldGroupType',
        Data : [
            {
                $Type: 'UI.DataField',
                Value: sensorID,
            },
            {
                $Type: 'UI.DataField',
                Value: temperatureInC,
            },
            {
                $Type: 'UI.DataField',
                Value: temperatureInF,
            },
            {
                $Type: 'UI.DataField',
                Value: humidity,
            },
            {
                $Type: 'UI.DataField',
                Value: createdAt,
            }
        ],
    },
    UI.Facets                    : [{
        $Type : 'UI.ReferenceFacet',
        ID    : 'GeneratedFacet1',
        Label : 'General Information',
        Target: '@UI.FieldGroup#GeneratedGroup',
    }, ],
    UI.LineItem                  : [
        {
            $Type: 'UI.DataField',
            Value: sensorID,
        },
        {
            $Type: 'UI.DataField',
            Value: temperatureInC,
        },
        {
            $Type: 'UI.DataField',
            Value: temperatureInF,
        },
        {
            $Type: 'UI.DataField',
            Value: humidity,
        },
        {
            $Type: 'UI.DataField',
            Value: createdAt,
        }
    ],
);
