import QtQuick 1.1
import "weatherQml" as Weather
import "weatherQml/Thunderstorms.js" as Logic
Item {
    id:baseWeather
    Component.onCompleted: Logic.init();

}
