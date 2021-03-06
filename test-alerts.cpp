#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include "test/catch.hpp"
#include "typewise-alert.h"

TEST_CASE("infers the breach according to limits") {
	REQUIRE(inferBreach(12, 20, 30) == TOO_LOW);
	REQUIRE(inferBreach(35, 20, 30) == TOO_HIGH);
	REQUIRE(inferBreach(15, 10, 20) == NORMAL);
}

TEST_CASE("classify the temperature breach according to limits") {
	REQUIRE(classifyTemperatureBreach(PASSIVE_COOLING, 10) == NORMAL);
	REQUIRE(classifyTemperatureBreach(PASSIVE_COOLING, -10) == TOO_LOW);
	REQUIRE(classifyTemperatureBreach(PASSIVE_COOLING, 37) == TOO_HIGH);

	REQUIRE(classifyTemperatureBreach(HI_ACTIVE_COOLING, 40) == NORMAL);
	REQUIRE(classifyTemperatureBreach(HI_ACTIVE_COOLING, -5) == TOO_LOW);
	REQUIRE(classifyTemperatureBreach(HI_ACTIVE_COOLING, 50) == TOO_HIGH);

	REQUIRE(classifyTemperatureBreach(MED_ACTIVE_COOLING, 30) == NORMAL);
	REQUIRE(classifyTemperatureBreach(MED_ACTIVE_COOLING, -1) == TOO_LOW);
	REQUIRE(classifyTemperatureBreach(MED_ACTIVE_COOLING, 45) == TOO_HIGH);
}

TEST_CASE("checks for the breach and and sends alerts") {
	BatteryCharacter testChar;
	testChar.coolingType = PASSIVE_COOLING;
	REQUIRE(checkAndAlert(TO_CONTROLLER, testChar, 10) == ALERT_SENT);
	testChar.coolingType = HI_ACTIVE_COOLING;
	REQUIRE(checkAndAlert(TO_CONTROLLER, testChar, -5) == ALERT_SENT);
	testChar.coolingType = MED_ACTIVE_COOLING;
	REQUIRE(checkAndAlert(TO_CONTROLLER, testChar, 45) == ALERT_SENT);

	testChar.coolingType = PASSIVE_COOLING;
	REQUIRE(checkAndAlert(TO_EMAIL, testChar, 50) == ALERT_SENT);
	testChar.coolingType = HI_ACTIVE_COOLING;
	REQUIRE(checkAndAlert(TO_EMAIL, testChar, -5) == ALERT_SENT);
	testChar.coolingType = MED_ACTIVE_COOLING;
	REQUIRE(checkAndAlert(TO_EMAIL, testChar, 45) == ALERT_SENT);

	testChar.coolingType = PASSIVE_COOLING;
	REQUIRE(checkAndAlert(TO_EMAIL, testChar, 10) == ALERT_NOT_REQUIRED);
	testChar.coolingType = HI_ACTIVE_COOLING;
	REQUIRE(checkAndAlert(TO_EMAIL, testChar, 40) == ALERT_NOT_REQUIRED);
	testChar.coolingType = MED_ACTIVE_COOLING;
	REQUIRE(checkAndAlert(TO_EMAIL, testChar, 30) == ALERT_NOT_REQUIRED);
}

TEST_CASE("Sends breach type message to controller when temperature breaches limits") {
	REQUIRE(sendToController(NORMAL) == ALERT_SENT);
	REQUIRE(sendToController(TOO_LOW) == ALERT_SENT);
	REQUIRE(sendToController(TOO_HIGH) == ALERT_SENT);
}

TEST_CASE("Sends breach type message to email when temperature breaches limits") {
	REQUIRE(sendToEmail(NORMAL) == ALERT_NOT_REQUIRED);
	REQUIRE(sendToEmail(TOO_LOW) == ALERT_SENT);
	REQUIRE(sendToEmail(TOO_HIGH) == ALERT_SENT);
}
