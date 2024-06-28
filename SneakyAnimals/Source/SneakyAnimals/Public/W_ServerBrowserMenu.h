// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "W_ServerBrowserMenu.generated.h"

/**
 * 
 */
UCLASS()
class SNEAKYANIMALS_API UW_ServerBrowserMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:
    UPROPERTY(EditAnywhere, Category = "MySettings", meta = (BindWidget))
	class UW_Base_Button* Back_Btn;
	
	UPROPERTY(EditAnywhere, Category = "MySettings", meta = (BindWidget))
	class UCircularThrobber* CircularThrobber;
	
	UPROPERTY(EditAnywhere, Category = "MySettings", meta = (BindWidget))
	class UCheckBox* bLAN_Checkbox;

    UPROPERTY(EditAnywhere, Category = "MySettings", meta = (BindWidget))
	class UW_Base_Button* Refresh_Btn;
	
	UPROPERTY(EditAnywhere, Category = "MySettings", meta = (BindWidget))
	class UTextBlock* ServerBrowser_Feedback_Text;
	
	UPROPERTY(EditAnywhere, Category = "MySettings", meta = (BindWidget))
	class UScrollBox* ServerList_Scrollbox;
	
	UPROPERTY(EditAnywhere, Category = "MySettings")
	int32 MaxResults_SessionsToFind = 50;
	
	UPROPERTY(EditAnywhere, Category = "MySettings")
	bool bSearchForLAN = false;

protected:
	virtual void NativeConstruct() override;

private:
	void RefreshServerList();
	void OnMyCheckStateChanged(bool IsChecked);
	void OnMyBack_BtnClicked();
	void FindSessions();
};
