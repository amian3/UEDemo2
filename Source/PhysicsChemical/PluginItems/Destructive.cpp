// Fill out your copyright notice in the Description page of Project Settings.


#include "Destructive.h"
#include "DestructibleComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "../BallPawn/BallPawn.h"
// Sets default values
ADestructive::ADestructive()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Item = CreateDefaultSubobject<UDestructibleComponent>(TEXT("Item"));
	//DetectBox = CreateDefaultSubobject<UBoxComponent>(TEXT("DetectBox"));
	RootComponent = Item;
	//DetectBox->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ADestructive::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADestructive::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (InDestroy)
	{
		DestroyTimer -= DeltaTime;
		if (DestroyTimer <= 0.0f)
		{
			Destroy();
		}
	}
}
/*
void ADestructive::OnDetectBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		ABallPawn* PlayerBall = Cast<ABallPawn>(OtherActor);
		if (PlayerBall && PlayerBall->BallStatus == ProperyStatus::Iron)
		{
			Item->ApplyDamage(10.0f, GetActorLocation(), OtherActor->GetActorForwardVector(), 1000.0f);
			Item->SetSimulatePhysics(true);
			//UGameplayStatics::ApplyDamage(this, 10.0f, PlayerBall->GetController(), this, TSubclassOf<UDamageType>());
			DestroyTimer = DestroyBound;
			InDestroy = true;
		}
	}
}
*/
void ADestructive::NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other,
	class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation,
	FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);
	//UE_LOG(LogTemp, Log, TEXT("NotifyHit"));
	
	if (Other)
	{
		ABallPawn* PlayerBall = Cast<ABallPawn>(Other);
		if (PlayerBall)
		{	
			if(PlayerBall->BallStatus == ProperyStatus::Iron)
			{
				Item->ApplyDamage(10.0f, GetActorLocation(), GetActorForwardVector(), 10000.0f);
				if (!InDestroy)
				{
					DestroyTimer = DestroyBound;
					InDestroy = true;
				}
			}
			else
			{
				PlayerBall->GetBall()->SetPhysicsLinearVelocity(
				PlayerBall->GetBall()->GetPhysicsLinearVelocity());
			}
		}
	}
}


