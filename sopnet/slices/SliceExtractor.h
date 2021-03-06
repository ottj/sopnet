#ifndef SOPNET_SLICE_EXTRACTOR_H__
#define SOPNET_SLICE_EXTRACTOR_H__

#include <deque>

#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>

#include <pipeline/all.h>
#include <inference/LinearConstraints.h>
#include <imageprocessing/ComponentTree.h>
#include <imageprocessing/MserParameters.h>
#include "Slices.h"

// forward declaration
class ComponentTreeDownSampler;
class Mser;
class MserParameters;

class SliceExtractor : public pipeline::ProcessNode {

public:

	SliceExtractor(unsigned int section);

private:

	class ComponentTreeConverter : public pipeline::SimpleProcessNode, public ComponentTree::Visitor {

	public:

		ComponentTreeConverter(unsigned int _section);

		void visitNode(boost::shared_ptr<ComponentTree::Node> node);

		void leaveNode(boost::shared_ptr<ComponentTree::Node> node);

	private:

		void addLinearConstraint();

		static unsigned int getNextSliceId();

		static unsigned int NextSliceId;

		static boost::mutex SliceIdMutex;

		void updateOutputs();

		void convert();

		pipeline::Input<ComponentTree>      _componentTree;
		pipeline::Output<Slices>            _slices;
		pipeline::Output<LinearConstraints> _linearConstraints;

		// the path to the currently visited component
		std::deque<unsigned int> _path;

		unsigned int _section;
	};

	// modifies the linear constraints according to the parameter 'force explanation'
	class LinearConstraintsFilter : public pipeline::SimpleProcessNode {

	public:

		LinearConstraintsFilter();

	private:

		void updateOutputs();

		pipeline::Input<LinearConstraints>  _linearConstraints;
		pipeline::Input<bool>               _forceExplanation;
		pipeline::Output<LinearConstraints> _filtered;
	};

	void onInputSet(const pipeline::InputSet<MserParameters>& signal);

	// optional mser parameters to override the program options
	pipeline::Input<MserParameters> _mserParameters;

	void extractSlices();

	boost::shared_ptr<Mser>                     _mser;
	boost::shared_ptr<MserParameters>           _defaultMserParameters;
	boost::shared_ptr<ComponentTreeDownSampler> _downSampler;
	boost::shared_ptr<ComponentTreeConverter>   _converter;
	boost::shared_ptr<LinearConstraintsFilter>  _filter;
};

#endif // SOPNET_SLICE_EXTRACTOR_H__

