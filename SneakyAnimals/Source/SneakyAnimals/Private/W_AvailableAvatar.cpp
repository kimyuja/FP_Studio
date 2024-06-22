#include "W_AvailableAvatar.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "W_ProfileMenu.h"

void UW_AvailableAvatar::OnClicked()
{
	// 나중에 오류 나면 if문 안에 넣기. 지금은 오류를 일부러 봐야 하는 단계임.
	//if (ParentWidget)
	//{
	//}
	class UW_ProfileMenu* profile = Cast<class UW_ProfileMenu>(ParentWidget);
	if (profile)
	{
		profile->Set_SelectedAvatar(Avatar_Name);
	}
}

void UW_AvailableAvatar::NativeConstruct()
{
	Super::NativeConstruct();

	if ( Avatar_Image && Avatar )
	{
		Avatar_Image->SetBrushFromTexture(Avatar);
	}

	if (Button)
	{
		Button->OnClicked.AddDynamic(this, &UW_AvailableAvatar::OnClicked);
	}
}
