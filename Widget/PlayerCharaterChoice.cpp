// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/PlayerCharaterChoice.h"

#include "EngineUtils.h"
#include "Components/Button.h"
#include "Components/SpotLightComponent.h"
#include "Components/TextBlock.h"
#include "Controllers/BasePlayerController.h"
#include "Engine/SpotLight.h"
#include "Frameworks/GameModes/BaseGameMode.h"
#include "Frameworks/GameStates/BaseGameState.h"
#include "Frameworks/PlayerStates/BasePlayerState.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY(PlayerCharaterChoiceWidget);

UPlayerCharaterChoice::UPlayerCharaterChoice(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<USoundWave> ShutterSoundAsset(TEXT("/Engine/VREditor/Sounds/UI/Camera_Shutter.Camera_Shutter"));

	if (ShutterSoundAsset.Succeeded())
	{
		GameStartSound = ShutterSoundAsset.Object;
	}
}

#pragma region Binding
void UPlayerCharaterChoice::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (GameStart)
	{
		GameStart->OnClicked.AddDynamic(this, &UPlayerCharaterChoice::GameStartingButtenEvent);
	}
}
#pragma endregion
#pragma region Left,RightKeyEvnet
void UPlayerCharaterChoice::LeftKeyEvent()
{
	ABasePlayerController* Owning = Cast<ABasePlayerController>(GetOwningPlayer());
	if (Owning->Client_0_Check) //Client 0 번 함수
	{
		TArray<AActor*> FoundControllers; //컨트롤러 전체를 가져와서 버튼 이벤를 동일하게 호출
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABasePlayerController::StaticClass(), FoundControllers);
		for (AActor* Controller : FoundControllers)
		{
			ABasePlayerController* PC = Cast<ABasePlayerController>(Controller);
			if (PC)
			{
				//Server_RPC호출
				PC->Server_Client_0_KeyEvent(0);
			}
		}
	}
	else
	{
		//위와 내용 동일 Client 1번 함수
		TArray<AActor*> FoundControllers;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABasePlayerController::StaticClass(), FoundControllers);
		for (AActor* Controller : FoundControllers)
		{
			ABasePlayerController* PC = Cast<ABasePlayerController>(Controller);
			if (PC)
			{
				PC->Server_Client_1_KeyEvent(0);
			}
		}
	}
	SpotLightCodyToGoMay();
}

void UPlayerCharaterChoice::RightKeyEvent()
{
	ABasePlayerController* Owning = Cast<ABasePlayerController>(GetOwningPlayer());
	if (Owning->Client_0_Check) //Client 0 번 함수
	{
		TArray<AActor*> FoundControllers; //컨트롤러 전체를 가져와서 버튼 이벤를 동일하게 호출
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABasePlayerController::StaticClass(), FoundControllers);
		for (AActor* Controller : FoundControllers)
		{
			ABasePlayerController* PC = Cast<ABasePlayerController>(Controller);
			if (PC)
			{
				//Server_RPC호출
				PC->Server_Client_0_KeyEvent(1);
			}
		}
	}
	else
	{
		//위와 내용 동일 Client 1번 함수
		TArray<AActor*> FoundControllers;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABasePlayerController::StaticClass(), FoundControllers);
		for (AActor* Controller : FoundControllers)
		{
			ABasePlayerController* PC = Cast<ABasePlayerController>(Controller);
			if (PC)
			{
				PC->Server_Client_1_KeyEvent(1);
			}
		}
	}
	SpotLightMayToGoCody();
}
#pragma endregion
#pragma region GameStartingButtenEvent
void UPlayerCharaterChoice::GameStartingButtenEvent()
{
	ABaseGameState* GS = GetWorld()->GetGameState<ABaseGameState>();
	if (GS)
	{
		ABasePlayerController* PC = Cast<ABasePlayerController>(GetOwningPlayer());
		//클라 0 / 1 동일 선택지이면 리턴
		if (GS->Client_0==GS->Client_1) 
		{return;}
		if (PC->Client_0_Check)
		{
			if (GS->Client_0==1||GS->Client_1==0)
			{
				UE_LOG(PlayerCharaterChoiceWidget, Display, TEXT("Cody"));
				PC->Server_PossessEvent(0);
			}
			else if (GS->Client_0==0||GS->Client_1==1)
			{
				UE_LOG(PlayerCharaterChoiceWidget, Display, TEXT("May"));
				PC->Server_PossessEvent(1);
			}
			else
			{
				UE_LOG(PlayerCharaterChoiceWidget, Display, TEXT("Fail"));
			}
			UGameplayStatics::PlaySound2D(this,GameStartSound);
			WidgetEixt(PC);
		}
		else
		{
			if (GS->Client_0==0||GS->Client_1==1)
			{
				UE_LOG(PlayerCharaterChoiceWidget, Display, TEXT("Cody"));
				PC->Server_PossessEvent(0);
			}
			else if (GS->Client_0==1||GS->Client_1==0)
			{
				UE_LOG(PlayerCharaterChoiceWidget, Display, TEXT("May"));
				PC->Server_PossessEvent(1);
			}
			else
			{
				UE_LOG(PlayerCharaterChoiceWidget, Display, TEXT("Fail"));
			}
			WidgetEixt(PC);
		}
	}
}

void UPlayerCharaterChoice::WidgetEixt(ABasePlayerController* PC)
{
	//캐릭터 선택이 완료되고 게임 시작전 조명 Off / 위젯 종료 / 마우스 포인트 Off
	UWorld* World = GetWorld();
	if (!World) return;
	for (TActorIterator<ASpotLight> It(World); It; ++It)
	{
		ASpotLight* Spot = *It;
		ASpotLight* SpotStop = *It;
		if (Spot && Spot->Tags.Contains(FName("May")))
		{
			ULightComponent* BaseLight = Spot->GetLightComponent();
			USpotLightComponent* SpotComp = Cast<USpotLightComponent>(BaseLight);
			if (SpotComp)
			{
				SpotComp->SetVisibility(false);
			}
		}
		if (Spot && Spot->Tags.Contains(FName("Cody")))
		{
			ULightComponent* BaseLight = Spot->GetLightComponent();
			USpotLightComponent* SpotComp = Cast<USpotLightComponent>(BaseLight);
			if (SpotComp)
			{
				SpotComp->SetVisibility(false);
			}
		}
	}
	PC->SetShowMouseCursor(false);
	this->RemoveFromParent();
}
#pragma endregion
#pragma region Left,RightSpotLightSetting
void UPlayerCharaterChoice::SpotLightCodyToGoMay()
{
	//월드에서 배치한 조명을 엔진 태그를 사용하여 조회 그이후 On/Off
	UWorld* World = GetWorld();
	if (!World) return;
	for (TActorIterator<ASpotLight> It(World); It; ++It)
	{
		ASpotLight* Spot = *It;
		ASpotLight* SpotStop = *It;
		if (Spot && Spot->Tags.Contains(FName("May")))
		{
			ULightComponent* BaseLight = Spot->GetLightComponent();
			USpotLightComponent* SpotComp = Cast<USpotLightComponent>(BaseLight);
			if (SpotComp)
			{
				SpotComp->SetVisibility(true);
			}
		}
		if (Spot && Spot->Tags.Contains(FName("Cody")))
		{
			ULightComponent* BaseLight = Spot->GetLightComponent();
			USpotLightComponent* SpotComp = Cast<USpotLightComponent>(BaseLight);
			if (SpotComp)
			{
				SpotComp->SetVisibility(false);
			}
		}
	}
}

void UPlayerCharaterChoice::SpotLightMayToGoCody()
{
	//월드에서 배치한 조명을 엔진 태그를 사용하여 조회 그이후 On/Off
	UWorld* World = GetWorld();
	if (!World) return;
	for (TActorIterator<ASpotLight> It(World); It; ++It)
	{
		ASpotLight* Spot = *It;
		ASpotLight* SpotStop = *It;
		if (Spot && Spot->Tags.Contains(FName("Cody")))
		{
			ULightComponent* BaseLight = Spot->GetLightComponent();
			USpotLightComponent* SpotComp = Cast<USpotLightComponent>(BaseLight);
			if (SpotComp)
			{
				SpotComp->SetVisibility(true);
			}
		}
		if (Spot && Spot->Tags.Contains(FName("May")))
		{
			ULightComponent* BaseLight = Spot->GetLightComponent();
			USpotLightComponent* SpotComp = Cast<USpotLightComponent>(BaseLight);
			if (SpotComp)
			{
				SpotComp->SetVisibility(false);
			}
		}
	}
}
#pragma endregion