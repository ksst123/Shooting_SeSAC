// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootingFlight.h"
#include "Components/BoxComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Bullet.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy.h"
#include "EngineUtils.h"
#include "ShootingGameModeBase.h"

// Sets default values
AShootingFlight::AShootingFlight()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// �浹 �ڽ� ������Ʈ ����
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	// ������ �浹 �ڽ� ������Ʈ�� ��Ʈ ������Ʈ�� ����
	RootComponent = BoxComponent; // == SetRootComponent(BoxComponent);
	// �ڽ� �ݸ��� ũ�⸦ (50, 50, 50) ���� ����
	BoxComponent->SetBoxExtent(FVector(50));
	// �ڽ� �ݸ����� �浹 ó�� �������� PlayerPreset ���� ����
	BoxComponent->SetCollisionProfileName(TEXT("PlayerPreset"));


	// �޽� ������Ʈ ����
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	// ������ �޽� ������Ʈ�� ��Ʈ ������Ʈ�� ���� ��ü�� ���
	MeshComponent->SetupAttachment(RootComponent);
	// Shape_Cube ������ ã�Ƽ� ������ ����
	ConstructorHelpers::FObjectFinder<UStaticMesh> CubeStaticMesh(TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));
	// ���� ť�� ������ �ε��ϴµ� �����ߴٸ�
	if (CubeStaticMesh.Succeeded())
	{
		// �ε��� ������ �޽� ������Ʈ�� Static Mesh �׸� �Ҵ�
		MeshComponent->SetStaticMesh(CubeStaticMesh.Object);
	}
	// MeshComponent->SetWorldLocation(FVector(0, 0, -50));

}

// Called when the game starts or when spawned
void AShootingFlight::BeginPlay()
{
	Super::BeginPlay();
	OriginMoveSpeed = MoveSpeed;
	
	// �÷��̾� ��Ʈ�ѷ��� ĳ����
	// APlayerController* PlayerController = Cast<APlayerController>(Controller);
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController != nullptr)
	{
		UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (SubSystem != nullptr)
		{
			SubSystem->AddMappingContext(IMC_MyMapping, 0);
		}
	}

	// ���� ���� �� ����
	/*PlayerMaterial = Cast<UMaterialInstanceDynamic>(MeshComponent->GetMaterial(0));
	FHashedMaterialParameterInfo Param;

	PlayerMaterial->GetVectorParameterValue(Param, InitColor);*/

	// ���� ���� �� ����
	UMaterialInterface* PlayerMaterial = MeshComponent->GetMaterial(0);
	FHashedMaterialParameterInfo Param = FHashedMaterialParameterInfo(TEXT("Color"));
	// Material Interface ���� ���� �Ķ���� ���� InitColor ������ ����
	PlayerMaterial->GetVectorParameterValue(Param, InitColor);

	UE_LOG(LogTemp, Warning, TEXT("R: %f, G: %f, B: %f"), InitColor.R, InitColor.G, InitColor.B);

	// material interface PlayerMaterial �� �������� Material instance dynamic ��ü�� ����
	DynamicMaterial = UMaterialInstanceDynamic::Create(PlayerMaterial, this);

	// ������ dynamic material �� �޽� ������Ʈ�� ����
	if (DynamicMaterial != nullptr)
	{
		MeshComponent->SetMaterial(0, DynamicMaterial);
	}
	
}

// Called every frame
void AShootingFlight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// direction ���� ����ȭ
	// ����ڰ� �Է��� ������ �̵�
	// P = P0 + vt == SetActorLocation = GetActorLocation + (direction(����) * speed(ũ��)) * DeltaTime
	// ����*ũ�� = �ӵ�, Scalar = �ӵ�, Delta�� ��� ���� �帧 �ǹ�
	direction.Normalize(); // ���̰� 1�� ���� = ���� ����
	FVector dir = GetActorLocation() + (direction * MoveSpeed) * DeltaTime;
	// bSweep = true, �̵� �� �浹 ü�� �ִ��� Sweep(���� ����) ���� true ����
	SetActorLocation(dir, true);

}

// Called to bind functionality to input
void AShootingFlight::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// ������ UInputComponent* ������ UEnhancedInputComponent �� ��ȯ
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	// �׼� �Լ��� ����
	EnhancedInputComponent->BindAction(IA_MoveHorizontal, ETriggerEvent::Triggered, this, &AShootingFlight::MoveHorizontal);
	EnhancedInputComponent->BindAction(IA_MoveHorizontal, ETriggerEvent::Completed, this, &AShootingFlight::MoveHorizontal);

	EnhancedInputComponent->BindAction(IA_MoveVertical, ETriggerEvent::Triggered, this, &AShootingFlight::MoveVertical);
	EnhancedInputComponent->BindAction(IA_MoveVertical, ETriggerEvent::Completed, this, &AShootingFlight::MoveVertical);

	EnhancedInputComponent->BindAction(IA_FireBullet, ETriggerEvent::Triggered, this, &AShootingFlight::FireBullet);

	EnhancedInputComponent->BindAction(IA_Boost, ETriggerEvent::Triggered, this, &AShootingFlight::BoostStarted);
	EnhancedInputComponent->BindAction(IA_Boost, ETriggerEvent::Completed, this, &AShootingFlight::BoostFinished);

	EnhancedInputComponent->BindAction(IA_ULT, ETriggerEvent::Triggered, this, &AShootingFlight::ExplosionAll);

	//// Horizontal Axis �Է¿� �Լ��� ����
	//PlayerInputComponent->BindAxis("MoveHorizontal", this, &AShootingFlight::MoveHorizontal);
	//// Vertical Axis �Է¿� �Լ��� ����
	//PlayerInputComponent->BindAxis("MoveVertical", this, &AShootingFlight::MoveVertical);

	//// Fire Action �Է¿� �Լ��� ����
	//PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AShootingFlight::FireBullet);
}

//// �¿� �Է��� ���� �� ����� �Լ�
//void AShootingFlight::MoveHorizontal(float value) {
//	
//	// �Է� value ���
//	h = value;
//	// UE_LOG(LogTemp, Warning, TEXT("h : %.4f"), h);
//
//	direction.Y = h;
//}
//
//// ���� �Է��� ���� �� ����� �Լ�
//void AShootingFlight::MoveVertical(float value) {
//	
//	// �Է� value ���
//	v = value;
//	// UE_LOG(LogTemp, Warning, TEXT("h : %.4f"), v);
//
//	direction.Z = v;
//}
//
//// ���콺 ���� ��ư�� ������ �� ����� �Լ�
//void AShootingFlight::FireBullet() {
//	
//	// Bullet ���� ��ġ�� ���� ���� ����
//	FVector SpawnPosition = GetActorLocation() + GetActorUpVector() * 90.0f;
//	// Bullet ���� �� ȸ������ ���� ���� ����
//	// �����Ϳʹ� �ٸ��� �ڵ忡���� FRotator(Pitch, Yaw, Roll) ����
//	FRotator SpawnRotation = FRotator(90.0f, 0, 0);
//	FActorSpawnParameters SpawnParam;
//	// ������Ʈ ���� �� ���� ��ġ�� �浹 ������ �ٸ� ������Ʈ�� ���� ��쿡 ��� �� ������ �����ϴ� �ɼ�
//	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
//
//	// Bullet�� ����
//	// Bullet Blueprint ����
//	GetWorld()->SpawnActor<ABullet>(BulletFactory, SpawnPosition, SpawnRotation, SpawnParam);
//}

void AShootingFlight::MoveHorizontal(const FInputActionValue& value) {
	h = value.Get<float>();
	// UE_LOG(LogTemp, Warning, TEXT("h : %.4f"), h);
	direction.Y = h;
}

void AShootingFlight::MoveVertical(const FInputActionValue& value) {
	v = value.Get<float>();
	// UE_LOG(LogTemp, Warning, TEXT("v : %.4f"), v);
	direction.Z = v;
}

void AShootingFlight::FireBullet() {
		
	if (IsTrapped)
	{
		return;
	}

	for (int32 i = 0; i < BulletCount; i++)
	{
		// �Ѿ��� ��ü ����
		float TotalSize = (BulletCount - 1) * BulletSpace;

		// ���� ��ġ
		float BaseY = TotalSize * -0.5f;

		// ���� ��ġ�� ������ ���� ����
		FVector Offset = FVector(0.0f, BaseY + BulletSpace * i, 0.0f);


		// Bullet ���� ��ġ�� ���� ���� ����
		FVector SpawnPosition = GetActorLocation() + GetActorUpVector() * 90.0f;
		SpawnPosition += Offset;
		// Bullet ���� �� ȸ������ ���� ���� ����
		// �����Ϳʹ� �ٸ��� �ڵ忡���� FRotator(Pitch, Yaw, Roll) ����
		FRotator SpawnRotation = FRotator(90.0f, 0, 0);
		FActorSpawnParameters SpawnParam;
		// ������Ʈ ���� �� ���� ��ġ�� �浹 ������ �ٸ� ������Ʈ�� ���� ��쿡 ��� �� ������ �����ϴ� �ɼ�
		SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		// Bullet�� ����
		// Bullet Blueprint ����
		ABullet* Bullet = GetWorld()->SpawnActor<ABullet>(BulletFactory, SpawnPosition, SpawnRotation, SpawnParam);


		// ������ Bullet �ν��Ͻ��� BulletAngle ��ŭ �����ϰ� ȸ����Ų��.
		float ExtraBulletTotalAngle = -0.5 * BulletAngle * (BulletCount - 1);
		FRotator OffsetAngle = FRotator(0.0f, 0.0f, ExtraBulletTotalAngle + BulletAngle * i);
		// Bullet->SetActorRelativeRotation(GetActorRotation() + OffsetAngle);
		if (Bullet != nullptr)
		{
			Bullet->AddActorWorldRotation(OffsetAngle);
		}
	}
	// Bullet �߻� ȿ���� ����
	UGameplayStatics::PlaySound2D(this, BulletFireSound);

	

	/*float BulletHalfSpace = BulletSpace / 2;
	int32 BulletHalfCount = BulletCount / 2;

		if (BulletCount == 1)
		{
			FVector SpawnPosition = GetActorLocation() + GetActorUpVector() * 90.0f;
			FRotator SpawnRotation = FRotator(90.0f, 0, 0);
			FActorSpawnParameters SpawnParam;
			GetWorld()->SpawnActor<ABullet>(BulletFactory, SpawnPosition, SpawnRotation, SpawnParam);
			UGameplayStatics::PlaySound2D(this, BulletFireSound);
		}
		else if (BulletCount % 2 == 0)
		{
			FVector ExtraSpawnPostion;
			float ExtraSpawnStartPosition = BulletHalfSpace + (BulletSpace * (BulletHalfCount / 2));
			ExtraSpawnPostion = FVector(0.0f, -1 * ExtraSpawnStartPosition, 0.0f);
			for (int i = 1; i <= BulletCount; i++)
			{
				FVector SpawnPosition = GetActorLocation() + GetActorUpVector() * 90.0f;
				FRotator SpawnRotation = FRotator(90.0f, 0, 0);
				FActorSpawnParameters SpawnParam;
				GetWorld()->SpawnActor<ABullet>(BulletFactory, SpawnPosition + ExtraSpawnPostion, SpawnRotation, SpawnParam);
				ExtraSpawnPostion += FVector(0.0f, BulletSpace, 0.0f);
			}
			UGameplayStatics::PlaySound2D(this, BulletFireSound);
		}
		else if (BulletCount % 2 != 0)
		{
			FVector ExtraSpawnPostion;
			float ExtraSpawnStartPosition = (BulletSpace * (BulletHalfCount));
			ExtraSpawnPostion = FVector(0.0f, -1 * ExtraSpawnStartPosition, 0.0f);
			for (int i = 1; i <= BulletCount; i++)
			{
				FVector SpawnPosition = GetActorLocation() + GetActorUpVector() * 90.0f;
				FRotator SpawnRotation = FRotator(90.0f, 0, 0);
				FActorSpawnParameters SpawnParam;
				GetWorld()->SpawnActor<ABullet>(BulletFactory, SpawnPosition + ExtraSpawnPostion, SpawnRotation, SpawnParam);
				ExtraSpawnPostion += FVector(0.0f, BulletSpace, 0.0f);
			}
			UGameplayStatics::PlaySound2D(this, BulletFireSound);
		}*/
}

void AShootingFlight::BoostStarted() {
	
	MoveSpeed *= 2;
}

void AShootingFlight::BoostFinished() {
	
	MoveSpeed = OriginMoveSpeed;
}

void AShootingFlight::ExplosionAll() {

	// ��� ���ʹ̸� �ı�(TActorIterator)
	/*for (TActorIterator<AEnemy> itr(GetWorld()); itr; ++itr)
	{
		itr->DestroyMyself();
	}*/


	// ��� ���ʹ̸� �ı�(�迭, ��������)
	//AShootingGameModeBase* MyGM = Cast<AShootingGameModeBase>(GetWorld()->GetAuthGameMode());
	//if (MyGM != nullptr)
	//{
	//	/*for (AEnemy* enemy : MyGM->EnemyArray)
	//	{
	//		enemy->DestroyMyself();
	//	}*/

	//	for (int32 i = 0; i < MyGM->EnemyArray.Num(); i++)
	//	{
	//		// IsValid�� ���� ��ӿ��� �� Pendingkill ���� Ȯ��
	//		if (IsValid(MyGM->EnemyArray[i]))
	//		{
	//			MyGM->EnemyArray[i]->DestroyMyself();
	//		}
	//	}
	//	// �迭�� �� �� �ʱ�ȭ
	//	MyGM->EnemyArray.Empty();
	//}


	// ��� ���ʹ̸� �ı�(��������Ʈ)
	// PlayerBomb.Broadcast();

	// ��� ���ʹ� �̵� ���� ����(��������Ʈ)
	FVector NewDir = (-1 * GetActorUpVector()) + GetActorRightVector();
	OnEnemyDirModify.Broadcast(NewDir);
}

// �浹 �� ���� 0.2�� ���� ����
//void AShootingFlight::ChangeColor() {
//	PlayerMaterial = Cast<UMaterialInstanceDynamic>(MeshComponent->GetMaterial(0));
//	
//	PlayerMaterial->SetVectorParameterValue(TEXT("Color"), FLinearColor::Red);
//}

void AShootingFlight::ChangeOriginColor() {
	//DynamicMaterial = Cast<UMaterialInstanceDynamic>(MeshComponent->GetMaterial(0));

	//DynamicMaterial->SetVectorParameterValue(TEXT("Color"), InitColor);
	//// PlayerMaterial->SetVectorParameterValue(TEXT("Color"), FLinearColor(255, 0, 0, 255);

	DynamicMaterial->SetVectorParameterValue(TEXT("Color"), InitColor);
}

void AShootingFlight::ReserveChangeColor(float time) {
	// ChangeColor();
	// ������ Red �� ����
	DynamicMaterial->SetVectorParameterValue(TEXT("Color"), (FVector4)FLinearColor::Red);

	GetWorld()->GetTimerManager().SetTimer(ColorTimer, this, &AShootingFlight::ChangeOriginColor, time, false);
}


// Boost Toggle
/*
*	void APlyerFlight::Boost()
*	{
*		IsBoosting = !IsBoosting;
* 
*		if (IsBoosting)
*		{
*			MoveSpeed = MoveSpeedOrigin * 2;
*		}
*		else
		{
			MoveSpeed = MoveSpeedOrigin;
		}

		// ���׿���
		// MoveSpeed = IsBoosting ? MoveSpeedOrigin*2 : MoveSpeedOrigin;
*
* 
*/