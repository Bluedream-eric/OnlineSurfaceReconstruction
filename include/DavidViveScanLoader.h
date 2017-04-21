#pragma once

#include "ScanLoader.h"
#include "AbstractViewer.h"
#include "DavidTurntableConnection.h"
#include <fstream>

#include <thread>

#include <openvr.h>

#include <boost/filesystem.hpp>

//Listens for user input on a vive controller and triggers a scan with the DAVID5 software
class DavidViveScanLoader : public ScanLoader
{
public:	
	DavidViveScanLoader(AbstractViewer* viewer);
	~DavidViveScanLoader();

	void setup(nanogui::Window*);

	void draw(const Matrix4f& mv, const Matrix4f& proj);

	bool mouseButtonEvent(const Eigen::Vector2i & p, int button, bool down, int modifiers);

	void feedback(Scan*);

private:
	enum State
	{
		Normal,
		Scanning,
		CalibratingShowScan,
		CalibratingShowViveController,
		CalibratingShowBoth,
	};
	State state;

	std::thread* trackingThread;
	bool tracking;

	vr::IVRSystem* vrSystem;

	void track();

	void Calibrate();
	void CalibrateTurntable(vr::TrackedDeviceIndex_t device);
	void TakeScan(const Eigen::Affine3f& transform);
	int FindOtherController(vr::TrackedDeviceIndex_t controller, vr::TrackedDevicePose_t* poses);
	bool ToEigenMatrix(const vr::TrackedDevicePose_t& pose, Eigen::Affine3f& m);
	bool waitUntilStill(vr::TrackedDevicePose_t* poses, vr::TrackedDeviceIndex_t device);

	Scan* viveController = nullptr;
	Scan* currentScan = nullptr;

	AbstractViewer* viewer;

	Camera::CamParams controllerCamParams, scanCamParams;
	
	std::vector<Vector3f> correspondencesSecondaryController, correspondencesDavidSystem;
	Eigen::Affine3f scannerControllerMatrix;
	Eigen::Affine3f secondaryControllerMatrix;
	Eigen::Affine3f transformScannerControllerToDavidSystem;

	double currentAngle;

	//turntable
	Vector3f axisCenter, axisDirection;

	boost::filesystem::path sessionPath;
	std::ofstream fEntireALN;
	std::string entireALNPath;
	int scansInALN;

	DavidTurntableConnection turntable;

	bool hasReference;
	Eigen::Affine3f referenceScanTransformInv; //used for feedback
	Eigen::Affine3f referenceScanUncalibratedInv;
	Eigen::Affine3f referenceDavidToVive;
	Eigen::Affine3f referenceTurntableRotation;
	Eigen::Matrix<float, 12, 12> calibrationAutoImproveMatrix;
	Eigen::Matrix<float, 12, 1> calibrationAutoImproveVector;
	Eigen::Affine3f adaptAfterCalibration;

	std::vector<Eigen::Matrix4f> Bs, Cs;
};