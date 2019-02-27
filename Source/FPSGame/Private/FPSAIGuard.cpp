// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSAIGuard.h"
#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"
#include "TimerManager.h"
#include "FPSGameMode.h"

// Sets default values
AFPSAIGuard::AFPSAIGuard()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComp = CreateDefaultSubobject< UPawnSensingComponent>(TEXT("PawnSensingComp"));
	PawnSensingComp->OnSeePawn.AddDynamic(this, &AFPSAIGuard::OnPawnSeen);
	PawnSensingComp->OnHearNoise.AddDynamic(this, &AFPSAIGuard::OnNoiseHear);


}

// Called when the game starts or when spawned
void AFPSAIGuard::BeginPlay()
{
	Super::BeginPlay();
	OriginalRotation = GetActorRotation();
	GuardState = EAIGuardState::Idle;
}

void AFPSAIGuard::OnPawnSeen(APawn * SeenPawn)
{
	if (SeenPawn == nullptr)
		return;

	DrawDebugSphere(GetWorld(), SeenPawn->GetActorLocation(), 30.0f, 12, FColor::Red,false,10.f);

	AFPSGameMode* GM = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());

	if (GM)
	{
		GM->CompliteMission(SeenPawn, false);
	}

	SetGuardState(EAIGuardState::Alerted);
}

void AFPSAIGuard::OnNoiseHear(APawn * NiseInstigator, const FVector & Location, float Volume)
{
	DrawDebugSphere(GetWorld(), Location, 30.0f, 5, FColor::Green, false, 10.f);

	FVector Direction = Location - GetActorLocation();
	Direction.Normalize();

	FRotator Rotator = FRotationMatrix::MakeFromX(Direction).Rotator();
	Rotator.Pitch = 0;
	Rotator.Roll = 0;

	SetActorRotation(Rotator);

	AActor::GetWorldTimerManager().SetTimer(TimerHandle_ResetOrientation,this,&AFPSAIGuard::ResetOrientation,3.0f);

	if(GuardState != EAIGuardState::Alerted)
		SetGuardState(EAIGuardState::Suspicios);
}

void AFPSAIGuard::ResetOrientation()
{
	if (GuardState == EAIGuardState::Alerted)
		return;

	//UE_LOG(LogTemp, Log, TEXT("Returned to default"));

	SetActorRotation(OriginalRotation);
	SetGuardState(EAIGuardState::Idle);
}

void AFPSAIGuard::SetGuardState(EAIGuardState state)
{
	if (state == GuardState)
		return;

	GuardState = state;

	OnStateChanged(state);
}

// Called every frame
void AFPSAIGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


