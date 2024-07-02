#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GI_SneakyAnimals.h"
#include "W_HostGameMenu.generated.h"

/**
 * 
 */
UCLASS()
class SNEAKYANIMALS_API UW_HostGameMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "MySettings", meta = (BindWidget))
	class UW_Base_Button* HostLobby_Btn;
	
	UPROPERTY(EditAnywhere, Category = "MySettings", meta = (BindWidget))
	class UW_Base_Button* Back_Btn;
	
	UPROPERTY(EditAnywhere, Category = "MySettings", meta = (BindWidget))
	class UW_Base_OptionSwitcher* MaxPlayer_Switcher;
	
	UPROPERTY(EditAnywhere, Category = "MySettings", meta = (BindWidget))
	class UW_Base_OptionSwitcher* ConnectionType_Switcher;

	UPROPERTY()
	bool bConnectionType_LAN;

	UPROPERTY()
	int32 MaxAmount_SessionConnections = 4;

    UFUNCTION()
    void OnHostLobby_BtnClicked();

    UFUNCTION()
    void OnBack_BtnClicked();

	UFUNCTION()
	void CreateLoadingScreen();

	UFUNCTION()
	void CreatePopUp();

	UPROPERTY(VisibleAnywhere, Category = "MyGameInstance")
	UGI_SneakyAnimals* gi;

protected:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void OnMaxPlayerSwitchedHandler(const FString& SelectedOption);
	
	UFUNCTION()
	void OnConnectionTypeSwitchedHandler(const FString& SelectedOption);

	UTexture2D* T_Color4_Normal;
	
	
};
