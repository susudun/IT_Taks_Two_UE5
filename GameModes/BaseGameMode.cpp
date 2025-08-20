// Fill out your copyright notice in the Description page of Project Settings.


#include "Frameworks/GameModes/BaseGameMode.h"

#include "EngineUtils.h"
#include "Camera/CameraActor.h"
#include "Controllers/BasePlayerController.h"
#include "Frameworks/GameStates/BaseGameState.h"
#include "Frameworks/PlayerStates/BasePlayerState.h"
#include "Characters/BaseCharacter.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY(LogGM);
ABaseGameMode::ABaseGameMode()
{
	bUseSeamlessTravel = true;
}

void ABaseGameMode::BeginPlay()
{
	Super::BeginPlay();
	if (GetCurrentMapNameAsFName() == "Begin_Third")
	{
		ChangeBGM();
	}
}
#pragma region PostLogin
void ABaseGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
#pragma region Check_Map and Player Save
	//기본 체크
	ABasePlayerController* PC = Cast<ABasePlayerController>(NewPlayer);
	ABasePlayerState* PS = Cast<ABasePlayerState>(PC->PlayerState);
	ABaseGameState* GS = GetWorld()->GetGameState<ABaseGameState>();
	//New_Player_Handling 
	if (!IsValid(PC))
	{
		//컨트롤러 확인 체크
		UE_LOG(LogGM, Error, TEXT("PostLogin: APlayer_Controller Cast False"));
		return;
	}
	if (PS)
	{
		//플레이어스테이트 저장
		if (GS->AllPlayerState.Num() >= 0&&GS->AllPlayerState.Num()<=2)
		{
			GS->AllPlayerState.Add(PS);	
		}
	}
	else
	{
		UE_LOG(LogGM, Error, TEXT("PostLogin: NewPlayer is PlayerState Cast ABasePlayerState False."));
	}
	if (!IsClient_0)
	{
		//접속자 0 / 1 번으로 체크를 위한
		PC->Client_Player_0Seting();
		IsClient_0 = true;
	}


	
#pragma endregion 
#pragma region Widget and Create Charater
	if (PS&&GS)
	{
		if (GameState->PlayerArray.Num() == 2)
		{
			for (APlayerState* PSW : GameState->PlayerArray)
			{
				ABasePlayerController* PCW = Cast<ABasePlayerController>(PSW->GetOwner());
				if (PCW)
				{
					PCW->Client_MakePlayerCharacterChoice(); // Client RPC
				}
			}
		}
		//배열안에 내PS값 전달
		int32 FoundIndex = GS->PlayerArray.IndexOfByKey(PS);
		//StartPoint 추출
		APlayerStart* StartPoint = Cast<APlayerStart>(UGameplayStatics::GetActorOfClass(GetWorld(), APlayerStart::StaticClass()));
		//찾아온 인덱스 번호가 유효하면 진행
		if (FoundIndex != INDEX_NONE)
		{
			switch (FoundIndex) // 플레이어 접속시 캐릭터 생성
			{
			case 0:
				if (StartPoint)
				{
					GetLavelSetingCamera(PC);
					SpawnCharacterMay(StartPoint,GS);
				}
				break;
	
			case 1:
				if (StartPoint)
				{
					GetLavelSetingCamera(PC);
					SpawnCharacterCody(StartPoint,GS);
				}
				break;
			default:
				break;
			}
		}
	}
#pragma endregion

	
}
#pragma endregion


#pragma region Server Traveling end In Game
void ABaseGameMode::HandleSeamlessTravelPlayer(AController*& C)
{
	Super::HandleSeamlessTravelPlayer(C);

	ABasePlayerController* PC = Cast<ABasePlayerController>(C);
	ABasePlayerState* PS = Cast<ABasePlayerState>(PC ? PC->PlayerState : nullptr);
	UE_LOG(LogGM, Warning, TEXT("CharaterNumber after SeamlessTravel: %d"), PS->CharaterNumber);
	if (!PC || !PS)
	{
		UE_LOG(LogGM, Error, TEXT("HandleSeamlessTravelPlayer: Invalid PC or PS"));
		return;
	}

	APlayerStart* StartPoint = Cast<APlayerStart>(UGameplayStatics::GetActorOfClass(GetWorld(), APlayerStart::StaticClass()));
	if (!StartPoint)
	{
		UE_LOG(LogGM, Error, TEXT("No PlayerStart found"));
		return;
	}

	FVector SpawnLocation = StartPoint->GetActorLocation();
	FRotator SpawnRotation = StartPoint->GetActorRotation();

	if (PS->CharaterNumber == 0)
	{
		SpawnLocation.Y += 280;
		ABaseCharacter* NewChar = GetWorld()->SpawnActor<ABaseCharacter>(BP_MayClass, SpawnLocation, SpawnRotation);
		if (NewChar)
		{
			PC->Possess(NewChar);
			UE_LOG(LogGM, Warning, TEXT("Spawned May after SeamlessTravel"));
		}
	}
	else if (PS->CharaterNumber == 1)
	{
		SpawnLocation.Y -= 320;
		ABaseCharacter* NewChar = GetWorld()->SpawnActor<ABaseCharacter>(BP_CodyClass, SpawnLocation, SpawnRotation);
		if (NewChar)
		{
			PC->Possess(NewChar);
			UE_LOG(LogGM, Warning, TEXT("Spawned Cody after SeamlessTravel"));
		}
	}
}

void ABaseGameMode::ChangeBGM()
{
	if (HasAuthority())
	{
		ABaseGameState* GS = GetGameState<ABaseGameState>();

		if (GS && BaseBGM)
		{
			GS->CurrentBGM = BaseBGM;
		}
	}
}
#pragma endregion
#pragma region Create Camera,Charater

FName ABaseGameMode::GetCurrentMapNameAsFName() const
{
	const FString CurrentMapNameString = UGameplayStatics::GetCurrentLevelName(GetWorld());
    
	// FString을 FName으로 변환하여 반환
	return FName(*CurrentMapNameString);
}

void ABaseGameMode::GetLavelSetingCamera(ABasePlayerController* PC)
{
	for (TActorIterator<ACameraActor> It(GetWorld()); It; ++It)
	{
		ACameraActor* Cam = *It;
		if (Cam)
		{
			if (Cam->Tags.Contains(FName("StartCamera")))
			{
				PC->SetViewTargetWithBlend(Cam, 0.f);
				break;
			}
		}
	}
}

void ABaseGameMode::SpawnCharacterMay(APlayerStart* StartPoint, ABaseGameState* GS)
{
	FVector SpawnLocation = StartPoint->GetActorLocation();
	FRotator SpawnRotation = StartPoint->GetActorRotation();
	SpawnLocation.Y+=280;
	ABaseCharacter* NewPlayerCharater = GetWorld()->SpawnActor<ABaseCharacter>(BP_MayClass, SpawnLocation, SpawnRotation);
	if (NewPlayerCharater)
	{
		NewPlayerCharater->SetReplicates(true);
		NewPlayerCharater->SetReplicateMovement(true);
	}
	GS->AllPlayerCharacter.Add(NewPlayerCharater);
	// NewPlayer->Possess(NewPlayerCharater);
}

void ABaseGameMode::SpawnCharacterCody(APlayerStart* StartPoint, ABaseGameState* GS)
{
	FVector SpawnLocation = StartPoint->GetActorLocation();
	FRotator SpawnRotation = StartPoint->GetActorRotation();
	SpawnLocation.Y -= 320.f;
	ABaseCharacter* NewPlayerCharacter = GetWorld()->SpawnActor<ABaseCharacter>(BP_CodyClass, SpawnLocation, SpawnRotation);
	if (NewPlayerCharacter)
	{
		NewPlayerCharacter->SetReplicates(true);
		NewPlayerCharacter->SetReplicateMovement(true);
	}
	GS->AllPlayerCharacter.Add(NewPlayerCharacter);
	// NewPlayer->Possess(NewPlayerCharacter);
}
#pragma endregion
#pragma region ServerTravel
void ABaseGameMode::HandleTravel(const FString& MapName)
{
	GetWorld()->ServerTravel(MapName,true);
}
#pragma endregion

