using drugSrv as service from '../../srv/drug-srv';
annotate service.temperature with @(
    UI.FieldGroup #GeneratedGroup : {
        $Type : 'UI.FieldGroupType',
        Data : [
            {
                $Type : 'UI.DataField',
                Label : 'sensorID',
                Value : sensorID,
            },
            {
                $Type : 'UI.DataField',
                Label : 'temperature',
                Value : temperature,
            },
            {
                $Type : 'UI.DataField',
                Label : 'createdAt',
                Value : createdAt,
            },
            {
                $Type : 'UI.DataField',
                Label : 'modifiedAt',
                Value : modifiedAt,
            },
        ],
    },
    UI.Facets : [
        {
            $Type : 'UI.ReferenceFacet',
            ID : 'GeneratedFacet1',
            Label : 'General Information',
            Target : '@UI.FieldGroup#GeneratedGroup',
        },
    ],
    UI.LineItem : [
        {
            $Type : 'UI.DataField',
            Label : 'sensorID',
            Value : sensorID,
        },
        {
            $Type : 'UI.DataField',
            Label : 'temperature',
            Value : temperature,
        },
        {
            $Type : 'UI.DataField',
            Label : 'createdAt',
            Value : createdAt,
        },
        {
            $Type : 'UI.DataField',
            Label : 'modifiedAt',
            Value : modifiedAt,
        },
    ],
);

