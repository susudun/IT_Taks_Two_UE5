// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/BasePlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Characters/BaseCharacter.h"
#include "Components/AudioComponent.h"
#include "Frameworks/GameStates/BaseGameState.h"
#include "Widget/PlayerCharaterChoice.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Frameworks/GameModes/BaseGameMode.h"
#include "Net/UnrealNetwork.h"
#include "Frameworks/GameInstances/ITTGameInstance.h"

DEFINE_LOG_CATEGORY(LogPC)

ABasePlayerController::ABasePlayerController()
{
	BGMAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("BGMAudioComponent"));
	BGMAudioComponent->bAutoActivate=false;
}

void ABasePlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (IsLocalController())
	{
		//델리게이트 바인딩
		ABaseGameState* GS = GetWorld()->GetGameState<ABaseGameState>();
		if (GS)
		{
			GS->OnClient_0CharacterChoice.AddDynamic(this,&ABasePlayerController::HandleClient_0CharaterChoice);
			GS->OnClient_1CharacterChoice.AddDynamic(this,&ABasePlayerController::HandleClient_1CharaterChoice);
		}
	}
}

void ABasePlayerController::HandleBGMChange(USoundBase* NewSound)
{
	if (!BGMAudioComponent || !NewSound) return;

	if (BGMAudioComponent->IsPlaying())
	{
		BGMAudioComponent->FadeOut(2,0);
	}

	BGMAudioComponent->SetSound(NewSound);
	BGMAudioComponent->FadeIn(2,1);
}

void ABasePlayerController::Debug_RespawnPlayer()
{
	UE_LOG(LogPC, Display, TEXT("Receive Delegate BroadCast."))
	if (not HasAuthority())
	{
		UE_LOG(LogPC, Display, TEXT("Run ServerRPC"))
		ServerRespawnAtLastCheckPoint();
	}
}

void ABasePlayerController::ServerRespawnAtLastCheckPoint_Implementation()
{
	UE_LOG(LogPC, Display, TEXT("ServerRespawnAtLastCheckPoint_Implementation()"));
	if (UITTGameInstance* GIS = Cast<UITTGameInstance>(GetGameInstance()))
	{
		GIS->RespawnPlayerCharacter(this);
	}
}

#pragma region Widget RPC
void ABasePlayerController::Client_MakePlayerCharacterChoice_Implementation()
{
	//위젯 생성
	PlayerSelectionWidget = CreateWidget<UPlayerCharaterChoice>(this, CharacterWidgetClass);
	if (!IsValid(PlayerSelectionWidget))
	{
		UE_LOG(LogTemp, Error, TEXT("Widget Create ERROR"));
		return;
	}
	PlayerSelectionWidget->AddToViewport();
	SetShowMouseCursor(true);
	UGameInstance* GI = GetWorld()->GetGameInstance();
	UITTGameInstance* ITTGI = Cast<UITTGameInstance>(GI);
	if (Client_0_Check)
	{
		PlayerSelectionWidget->Player_Name_1->SetText(FText::FromString(ITTGI->CachedUserName));
	}
	else
	{
		PlayerSelectionWidget->Player_Name->SetText(FText::FromString(ITTGI->CachedUserName));
	}
}

void ABasePlayerController::ConnectToServer(const FString& IPAddress)
{
	UE_LOG(LogTemp, Warning, TEXT("IP Address is %s"), *IPAddress);
	ClientTravel(IPAddress,TRAVEL_Absolute);
}

void ABasePlayerController::HandleClient_0CharaterChoice(int32 MayOrCody)
{
	//위젯 이미지 위치 설정
	if (IsValid(PlayerSelectionWidget) && IsValid(PlayerSelectionWidget->Client_0))
	{
		const float XOffset = MayOrCody > 0  ? 450.f : -450.f;
		PlayerSelectionWidget->Client_0->SetRenderTranslation(FVector2D(XOffset, 0.f));
		PlayerSelectionWidget->Player_Name->SetRenderTranslation(FVector2D(XOffset, 0.f));
	}
}

void ABasePlayerController::HandleClient_1CharaterChoice(int32 MayOrCody)
{
	//위젯 이미지 위치 설정
	if (IsValid(PlayerSelectionWidget) && IsValid(PlayerSelectionWidget->Client_1))
	{
		const float XOffset = MayOrCody > 0  ? 450.f : -450.f;
		PlayerSelectionWidget->Client_1->SetRenderTranslation(FVector2D(XOffset, 0.f));
		PlayerSelectionWidget->Player_Name_1->SetRenderTranslation(FVector2D(XOffset, 0.f));
	}
}

void ABasePlayerController::Server_PlayerChoiceSave_Implementation(int32 Number)
{
	ABasePlayerState* PlayerCharaterNumber=GetPlayerState<ABasePlayerState>();
	if (PlayerCharaterNumber)
	{
		PlayerCharaterNumber->CharaterNumber=Number;
	}
}


void ABasePlayerController::Server_RequestTravel_Implementation(const FString& MapName)
{
	ABaseGameMode* GM = Cast<ABaseGameMode>(GetWorld()->GetAuthGameMode());
	if (GM)
	{
		GM->HandleTravel(MapName);
	}
}

void ABasePlayerController::Server_PossessEvent_Implementation(int32 MayOrCody)
{
	//선택 캐릭터 빙의
	ABaseGameState* GS = GetWorld()->GetGameState<ABaseGameState>();
	if (GS && GS->AllPlayerCharacter.IsValidIndex(MayOrCody))
	{
		// Possess(GS->AllPlayerCharacter[MayOrCody]);
	}
	ABaseGameMode* GM = Cast<ABaseGameMode>(GetWorld()->GetAuthGameMode());
	if (GM)
	{
		ABasePlayerState* PlayerCharaterNumber=GetPlayerState<ABasePlayerState>();
		bool bisMayCody = MayOrCody == 0 ? true : false;
		if (bisMayCody)
		{
			GM->MayPossessed=true;
			Server_PlayerChoiceSave(0);
		}
		else
		{
			GM->CodyPossessed=true;
			Server_PlayerChoiceSave(1);
		}
		if (GM->MayPossessed&&GM->CodyPossessed)
		{
			Server_RequestTravel("/Game/Maps/Begin/Begin_Third?listen");
		}
	}
}

void ABasePlayerController::Server_Client_1_KeyEvent_Implementation(int32 LeftOrRight)
{
	ABaseGameState* GS = GetWorld()->GetGameState<ABaseGameState>();
	if(!GS)UE_LOG(LogTemp, Warning, TEXT("GS_ServerRPC_ERROR"));
	GS->Client_0=LeftOrRight;
}

void ABasePlayerController::Server_Client_0_KeyEvent_Implementation(int32 LeftOrRight)
{
	ABaseGameState* GS = GetWorld()->GetGameState<ABaseGameState>();
	if(!GS)UE_LOG(LogTemp, Warning, TEXT("GS_ServerRPC_ERROR"));
	GS->Client_1=LeftOrRight;
}

void ABasePlayerController::Client_Player_0Seting_Implementation()
{
	Client_0_Check = true;
}
#pragma endregion