
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "I_LobbyToggleInterface.generated.h"

UINTERFACE(MinimalAPI)
class UI_LobbyToggleInterface : public UInterface
{
	GENERATED_BODY()
};

class SNEAKYANIMALS_API II_LobbyToggleInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Action")
	void Toggle_CharacterCustomization();

	virtual void Toggle_CharacterCustomization_Implementation() {};
};
