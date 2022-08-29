// Fill out your copyright notice in the Description page of Project Settings.


#include "GravityBall.h"
#include "../BallPawn/BallPawn.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "BasicActor.h"
#include "Kismet/KismetMathLibrary.h"
#include "PhysicsCube.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Particles/ParticleSystemComponent.h"
// Sets default values
AGravityBall::AGravityBall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Sphere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sphere"));
	RootComponent = Sphere;
	Boundary = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Boundary"));
	Boundary->SetupAttachment(RootComponent);
	DetectSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DetectSphere"));
	DetectSphere->SetupAttachment(RootComponent);
	DeactivateGravity();

	Partic = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Partic"));
	Partic->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AGravityBall::BeginPlay()
{
	Super::BeginPlay();
	
	DetectSphere->OnComponentBeginOverlap.AddDynamic(this, &AGravityBall::OnDetectSphereOverlapBegin);
	DetectSphere->OnComponentEndOverlap.AddDynamic(this, &AGravityBall::OnDetectSphereOverlapEnd);

	TArray<AActor*> ignoreChars;
	TArray<AActor*> destActors;
	TArray<TEnumAsByte<EObjectTypeQuery>> destObjectTypes;
	destObjectTypes.Add((EObjectTypeQuery)::ECollisionChannel::ECC_WorldDynamic);
	UKismetSystemLibrary::SphereOverlapActors(
		GetWorld(), GetActorLocation(), 2000.0f,
		destObjectTypes, ABasicActor::StaticClass(), ignoreChars, destActors
	);
	for (int i = 0; i < destActors.Num(); ++i)
	{
		Satellites.Add(destActors[i]);
	}

}

// Called every frame
void AGravityBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (WithGravity)
	{
		for (int i = 0; i < Satellites.Num(); ++i)
		{
			ApplyGravityToPlayer(Satellites[i]);
		}
	}


}

void AGravityBall::ActivateGravity()
{
	WithGravity = true;
	Boundary->SetVisibility(true);
	Sphere->SetVisibility(false);
}

void AGravityBall::DeactivateGravity()
{
	WithGravity = false;
	Boundary->SetVisibility(false);
	Sphere->SetVisibility(true);
}

bool AGravityBall::IsGravity()
{
	return WithGravity;
}

void AGravityBall::ApplyGravityToPlayer(AActor* Satellite)
{
	if (Satellite)
	{
		ABasicActor* Sate = Cast<ABasicActor>(Satellite);
		if (Sate)
		{
			if (Sate->GetStaticMesh())
			{
				FVector Dir = GetActorLocation() - Sate->GetActorLocation();
				float Scale = GravityConstant / powf(Dir.Size() - DistanceConstant, 2);

				Sate->GetStaticMesh()->AddForce(Scale * Dir.GetSafeNormal());
				
			}
		}
		else
		{
			ABallPawn* PlayerBall = Cast<ABallPawn>(Satellite);
			if (PlayerBall && PlayerBall->GetBall())
			{
				FVector Dir = GetActorLocation() - PlayerBall->GetActorLocation();
				float Scale = GravityConstant / Dir.SizeSquared();

				PlayerBall->GetBall()->AddForce(Scale * Dir.GetSafeNormal());
				//UE_LOG(LogTemp, Log, TEXT("%f%f%f"), Dir.X, Dir.Y, Dir.Z);
			}
		}

	}
}

void AGravityBall::OnDetectSphereOverlapBegin(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		Satellites.Add(OtherActor);
	}
}

void AGravityBall::OnDetectSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	int Index = Satellites.Find(OtherActor);
	if (Index != -1)
	{
		Satellites.RemoveAt(Index);
	}
}

void AGravityBall::OnClickGravityBall()
{
	if (WithGravity)
	{
		DeactivateGravity();
	}
	else
	{
		ActivateGravity();
	}
}

