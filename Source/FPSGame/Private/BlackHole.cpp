// Fill out your copyright notice in the Description page of Project Settings.

#include "BlackHole.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ABlackHole::ABlackHole()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = MeshComp;

	InnerSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("InnerSphereComp"));
	InnerSphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	InnerSphereComponent->SetSphereRadius(100);
	InnerSphereComponent->SetupAttachment(MeshComp);

	InnerSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ABlackHole::OverlapInnerSphere);

	OuterSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("OuterSphereComp"));
	OuterSphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OuterSphereComponent->SetSphereRadius(1500);
	OuterSphereComponent->SetupAttachment(MeshComp);
}


void ABlackHole::OverlapInnerSphere(UPrimitiveComponent * OverlapedComponents, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{

	if (OtherActor)
	{
		OtherActor->Destroy();
	}
}

// Called every frame
void ABlackHole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<UPrimitiveComponent*> OverlappingComponents;
	OuterSphereComponent->GetOverlappingComponents(OverlappingComponents);

	for (int32 i = 0; i < OverlappingComponents.Num(); ++i)
	{
		UPrimitiveComponent* PrimComp = OverlappingComponents[i];

		if (PrimComp && PrimComp->IsSimulatingPhysics())
		{
			float ShapeRadius = OuterSphereComponent->GetScaledSphereRadius();
			PrimComp->AddRadialForce(GetActorLocation(), ShapeRadius, -2000, ERadialImpulseFalloff::RIF_Constant, true);
		}
	}

}

