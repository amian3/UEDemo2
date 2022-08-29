// Fill out your copyright notice in the Description page of Project Settings.


#include "RebornManager.h"
#include "Components/BoxComponent.h"
#include "BallPawn.h"
// Sets default values
ARebornManager::ARebornManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	DetectBox = CreateDefaultSubobject<UBoxComponent>(TEXT("DetectBox"));
	RootComponent = DetectBox;
}

// Called when the game starts or when spawned
void ARebornManager::BeginPlay()
{
	Super::BeginPlay();
	DetectBox->OnComponentBeginOverlap.AddDynamic(this, &ARebornManager::OnDetectBoxOverlapBegin);
	DetectBox->OnComponentEndOverlap.AddDynamic(this, &ARebornManager::OnDetectBoxOverlapEnd);
}

// Called every frame
void ARebornManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ARebornManager::OnDetectBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		ABallPawn* PlayerBall = Cast<ABallPawn>(OtherActor);
		if (PlayerBall)
		{
			if (PlayerBall->ReBornPriority < RebornPriority)
			{
				PlayerBall->ReBornLocation = RebornLocation;
			}
		}
	}
}


void ARebornManager::OnDetectBoxOverlapEnd(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		ABallPawn* PlayerBall = Cast<ABallPawn>(OtherActor);
		if (PlayerBall)
		{
			if (PlayerBall->GetActorLocation().Z < -1500.0f)
			{
				PlayerBall->SetActorLocation(PlayerBall->ReBornLocation);
				PlayerBall->GetStaticMesh()->SetPhysicsLinearVelocity(FVector(0.0f, 0.0f, 0.0f));
				PlayerBall->GetStaticMesh()->SetPhysicsAngularVelocity(FVector(0.0f, 0.0f, 0.0f));
			}
		}
	}
}

