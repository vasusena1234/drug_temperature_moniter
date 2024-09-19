sap.ui.require(
    [
        'sap/fe/test/JourneyRunner',
        'com/drugs/tempmoniter/test/integration/FirstJourney',
		'com/drugs/tempmoniter/test/integration/pages/temperatureList',
		'com/drugs/tempmoniter/test/integration/pages/temperatureObjectPage'
    ],
    function(JourneyRunner, opaJourney, temperatureList, temperatureObjectPage) {
        'use strict';
        var JourneyRunner = new JourneyRunner({
            // start index.html in web folder
            launchUrl: sap.ui.require.toUrl('com/drugs/tempmoniter') + '/index.html'
        });

       
        JourneyRunner.run(
            {
                pages: { 
					onThetemperatureList: temperatureList,
					onThetemperatureObjectPage: temperatureObjectPage
                }
            },
            opaJourney.run
        );
    }
);