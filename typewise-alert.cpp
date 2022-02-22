#include "typewise-alert.h"
#include "breach-alerts-map.h"
#include <stdio.h>

BreachType inferBreach(double value, double lowerLimit, double upperLimit) {
  if(value < lowerLimit) {
    return TOO_LOW;
  }
  if(value > upperLimit) {
    return TOO_HIGH;
  }
  return NORMAL;
}

funcptr SenderFunctionFinder(AlertTarget alertTarget)
{
	return AlertTargetBreachSenderFunction[alertTarget];
}

BreachType classifyTemperatureBreach(
    CoolingType coolingType, double temperatureInC) {
	return inferBreach(temperatureInC, TemperatureLimitsMap[coolingType].at(0), TemperatureLimitsMap[coolingType].at(1));
}

void checkAndAlert(
	AlertTarget alertTarget, BatteryCharacter batteryChar, double temperatureInC) {

  BreachType breachType = classifyTemperatureBreach(
    batteryChar.coolingType, temperatureInC
  );

  funcptr breachSender = SenderFunctionFinder(alertTarget);
  (*breachSender)(breachType);
}

void sendToController(BreachType breachType) {
  const unsigned short header = 0xfeed;
  printf("%x : %x\n", header, breachType);
}

void sendToEmail(BreachType breachType) {
  const char* recepient = "a.b@c.com";
  if (breachType != NORMAL)
  {
	  const char* tempratureBreachMessage = TemperatureBreachMessageMap[breachType];
	  printf("To: %s\n", recepient);
	  printf("%s\n", tempratureBreachMessage);
  }
}
