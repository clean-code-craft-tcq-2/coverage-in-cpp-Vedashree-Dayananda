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

AlertStatus checkAndAlert(
	AlertTarget alertTarget, BatteryCharacter batteryChar, double temperatureInC) {

	AlertStatus result = ALERT_NOT_SENT;
	
	BreachType breachType = classifyTemperatureBreach(
		batteryChar.coolingType, temperatureInC);

  funcptr breachSender = SenderFunctionFinder(alertTarget);
  result = (*breachSender)(breachType);
  return result;
}

AlertStatus sendToController(BreachType breachType) {
  const unsigned short header = 0xfeed;
  AlertStatus result = ALERT_NOT_SENT;
  printf("%x : %x\n", header, breachType);
  result = ALERT_SENT;
  return result;
}

AlertStatus sendToEmail(BreachType breachType) {
  const char* recepient = "a.b@c.com";
  AlertStatus result = ALERT_NOT_REQUIRED;
  if (breachType != NORMAL)
  {
	  const char* tempratureBreachMessage = TemperatureBreachMessageMap[breachType];
	  printf("To: %s\n", recepient);
	  printf("%s\n", tempratureBreachMessage);
	  result = ALERT_SENT;
  }
  return result;
}
