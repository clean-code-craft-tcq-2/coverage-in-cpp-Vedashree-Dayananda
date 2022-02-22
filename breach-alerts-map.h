#include <map>
#include<vector>

std::map < CoolingType, std::vector<int>> TemperatureLimitsMap = {
	{ PASSIVE_COOLING, { 0, 35 } },
	{ HI_ACTIVE_COOLING, { 0, 45 } },
	{ MED_ACTIVE_COOLING, { 0, 40 } }
};

std::map < BreachType, const char*> TemperatureBreachMessageMap = {
	{ TOO_LOW, "The temperature is too low" },
	{ TOO_HIGH, "the temperature is too high" }
};

using funcptr = void(*)(BreachType);

std::map < AlertTarget, funcptr > AlertTargetBreachSenderFunction = {
	{ TO_CONTROLLER, sendToController },
	{ TO_EMAIL, sendToEmail }
};
