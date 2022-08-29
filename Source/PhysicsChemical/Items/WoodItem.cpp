// Fill out your copyright notice in the Description page of Project Settings.


#include "WoodItem.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "../BallPawn/BallPawn.h"
// Sets default values
AWoodItem::AWoodItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Cube = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube"));
	RootComponent = Cube;
	Block = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Block"));
	Block->SetupAttachment(Cube);
	DetectBox = CreateDefaultSubobject<UBoxComponent>(TEXT("DetectSphere"));
	DetectBox->SetupAttachment(RootComponent);

	FireEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("FireEffect"));
	SmokeEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("SmokeEffect"));

	FireEffect->SetupAttachment(RootComponent);
	SmokeEffect->SetupAttachment(RootComponent);


}

// Called when the game starts or when spawned
void AWoodItem::BeginPlay()
{
	Super::BeginPlay();

	DetectBox->OnComponentBeginOverlap.AddDynamic(this, &AWoodItem::OnDetectBoxOverlapBegin);
	DetectBox->OnComponentEndOverlap.AddDynamic(this, &AWoodItem::OnDetectBoxOverlapEnd);

	FireEffect->Deactivate();
	SmokeEffect->Deactivate();
}

// Called every frame
void AWoodItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (BStatus == BurstStatus::Prepare)
	{
		BroadCastTimer -= DeltaTime;
		if (BroadCastTimer <= 0.0f)
		{
			BStatus = BurstStatus::BroadCast;
			BurstOutTimer = BurstOutBound;
			TArray<AActor*> ignoreChars;
			TArray<AActor*> destActors;
			TArray<TEnumAsByte<EObjectTypeQuery>> destObjectTypes;
			destObjectTypes.Add((EObjectTypeQuery)::ECollisionChannel::ECC_WorldDynamic);
			UKismetSystemLibrary::SphereOverlapActors(
				GetWorld(), GetActorLocation(), 500.0f,
				destObjectTypes, AWoodItem::StaticClass(), ignoreChars, destActors
			);
			for (int i = 0; i < destActors.Num(); ++i)
			{
				if (rand() % 2)
				{
					AWoodItem* CurrentWood = Cast<AWoodItem>(destActors[i]);
					if (CurrentWood && CurrentWood != this && CurrentWood->BStatus == BurstStatus::Normal)
					{
						CurrentWood->SetBurn();
					}
				}

			}
		}
	}
	else if (BStatus == BurstStatus::BroadCast)
	{
		if (!SmokeEffect->IsActive())
		{
			SmokeEffect->Activate(true);
		}
		BurstOutTimer -= DeltaTime;
		if (BurstOutTimer <= 0.0f)
		{
			Destroy();
		}
	}

}

void AWoodItem::SetBurn()
{
	BroadCastTimer = BroadCastBound;
	BStatus = BurstStatus::Prepare;
	FireEffect->Activate(true);
}

void AWoodItem::OnDetectBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		ABallPawn* PlayerBall = Cast<ABallPawn>(OtherActor);
		if (PlayerBall && PlayerBall->BallStatus == ProperyStatus::Fire)
		{
			SetBurn();
		}
	}
}

void AWoodItem::OnDetectBoxOverlapEnd(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{/*
	if (OtherActor)
	{
		ABallPawn* PlayerBall = Cast<ABallPawn>(OtherActor);
		if (PlayerBall)
		{
			if (StartBurnTimer > 0)
			{
				BStatus = BurstStatus::Normal;
			}
		}
	}
*/
}