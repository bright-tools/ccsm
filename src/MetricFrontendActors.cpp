/*
   Copyright 2015 John Bailey

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/
#include "MetricFrontendActors.hpp"
#include "MetricASTConsumer.hpp"
#include "MetricPPCustomer.hpp"

#include "clang/Frontend/CompilerInstance.h"

class ASTMetricConsumerFactory : public clang::ASTFrontendAction {
protected:
    MetricOptions*           m_options;
    MetricUnit*              m_topUnit;
    SrcStartToFunctionMap_t* m_srcMap;
public:
    ASTMetricConsumerFactory( MetricOptions* p_options, MetricUnit* p_topUnit, SrcStartToFunctionMap_t* p_srcMap );
	virtual ~ASTMetricConsumerFactory();
};

ASTMetricConsumerFactory::ASTMetricConsumerFactory( MetricOptions* p_options, MetricUnit* p_topUnit, SrcStartToFunctionMap_t* p_srcMap )
	: m_options( p_options ), m_topUnit( p_topUnit ), m_srcMap( p_srcMap )
{

}

ASTMetricConsumerFactory::~ASTMetricConsumerFactory()
{
}

class ASTMetricFrontendActionFactory : public ASTMetricConsumerFactory {
protected:
	std::set<std::string>* m_commentFileList;
public:
    ASTMetricFrontendActionFactory( MetricOptions* p_options, MetricUnit* p_topUnit, SrcStartToFunctionMap_t* p_srcMap, std::set<std::string>* p_commentFileList );
    virtual std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance &CI, llvm::StringRef file);
	virtual ~ASTMetricFrontendActionFactory();
};

ASTMetricFrontendActionFactory::ASTMetricFrontendActionFactory( MetricOptions* p_options, MetricUnit* p_topUnit, SrcStartToFunctionMap_t* p_srcMap, std::set<std::string>* p_commentFileList )
	: m_commentFileList( p_commentFileList), ASTMetricConsumerFactory( p_options, p_topUnit , p_srcMap)
{
}

ASTMetricFrontendActionFactory::~ASTMetricFrontendActionFactory()
{
}

std::unique_ptr<clang::ASTConsumer> ASTMetricFrontendActionFactory::CreateASTConsumer(clang::CompilerInstance &CI, llvm::StringRef file) {
	std::unique_ptr<clang::ASTConsumer> ret_val = llvm::make_unique<MetricASTConsumer>(CI,m_topUnit,m_options,m_srcMap);
	MetricPPCustomer* customer = new MetricPPCustomer( m_topUnit, m_commentFileList, m_options, m_srcMap );
    CI.getPreprocessor().addCommentHandler(customer);
    CI.getPreprocessor().addPPCallbacks(llvm::make_unique<MetricPPCustomer>(*customer));
    return ret_val;
}

class ASTFrontendActionFactory : public clang::tooling::FrontendActionFactory {
   protected:
    MetricOptions*           m_options;
    MetricUnit*              m_topUnit;
    SrcStartToFunctionMap_t* m_srcMap;
	std::set<std::string>* m_commentFileList;
   public:
     ASTFrontendActionFactory( MetricOptions* p_options, MetricUnit* p_topUnit, SrcStartToFunctionMap_t* p_srcMap, std::set<std::string>* p_commentFileList )
	      : m_options( p_options), m_topUnit( p_topUnit ), m_srcMap( p_srcMap ), m_commentFileList( p_commentFileList ), clang::tooling::FrontendActionFactory()
	 {
	 }
	 clang::FrontendAction *create() override { 
		 return new ASTMetricFrontendActionFactory( m_options, m_topUnit, m_srcMap, m_commentFileList ); 
	 }
	 virtual ~ASTFrontendActionFactory()
	 {
	 }
};

clang::tooling::FrontendActionFactory* newASTMetricFrontendActionFactory( MetricOptions* p_options, MetricUnit* p_topUnit, SrcStartToFunctionMap_t* p_srcMap, std::set<std::string>* p_commentFileList  ) {
   return new ASTFrontendActionFactory( p_options, p_topUnit, p_srcMap, p_commentFileList );
}

class PPFrontendActionFactory : public clang::tooling::FrontendActionFactory {
   protected:
    MetricOptions*           m_options;
    MetricUnit*              m_topUnit;
    SrcStartToFunctionMap_t* m_srcMap;
   public:
     PPFrontendActionFactory( MetricOptions* p_options, MetricUnit* p_topUnit, SrcStartToFunctionMap_t* p_srcMap )
	      : m_options( p_options), m_topUnit( p_topUnit ), m_srcMap( p_srcMap ), clang::tooling::FrontendActionFactory()
	 {
	 }

	 clang::FrontendAction *create() override { 
		 return new MetricPPConsumer( m_topUnit, m_options, m_srcMap ); 
	 }

	 virtual ~PPFrontendActionFactory()
	 {
	 }
};

clang::tooling::FrontendActionFactory* newPPMetricFrontendActionFactory( MetricOptions* p_options, MetricUnit* p_topUnit, SrcStartToFunctionMap_t* p_srcMap  ) {
   return new PPFrontendActionFactory( p_options, p_topUnit, p_srcMap );
}
